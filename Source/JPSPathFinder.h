#pragma once
#include <stdint.h>
#include <memory>		
#include <string>
#include <vector>
#include <list>
#include <cmath>



namespace jpspath {


	template<typename _Ty>
	_Ty clamp(const _Ty& val, const _Ty& min, const _Ty& max)
	{
		return std::max(min, std::min(max, val));
	}


	//--------------------------------------------------------------------
	// Class: BitArray
	// Desc	: 2 차원 BitArray Class
	//--------------------------------------------------------------------
	template <typename _Ty>
	class BitArray : public std::vector<_Ty>
	{
	public:
		div_t	div(int32_t pos) { return ::div(pos, __NBITMASK); }
		_Ty		divmaskbits(const div_t& dv) { return (_Ty)1 << dv.rem; }
		int32_t	bitsposition(int32_t x, int32_t y) { return (int32_t)(y * m_bitswidths + x); }

		int32_t	m_bitswidths = 0;
		int32_t	m_wordwidths = 0;

		int32_t	m_width = 0;
		int32_t	m_depth = 0;

	public:
		static const int32_t	__NBITMASK = static_cast<int32_t>(8 * sizeof(_Ty));	// 64bit
		static const _Ty	__FULLBITS = std::numeric_limits<_Ty>::max();
		static const _Ty	__CLEARBITS = 0;

		auto	GetBitsWidths() const-> decltype(m_bitswidths) { return m_bitswidths; }
		auto	GetWordWidths() const-> decltype(m_wordwidths) { return m_wordwidths; }
		auto	GetWidth()		const->decltype(m_width) { return m_width; }
		auto	GetHeight()		const->decltype(m_depth) { return m_depth; }


		//--------------------------------------------------------------------
		// Class: Create()
		// Desc : width, height 크기로 설정합니다.
		//--------------------------------------------------------------------
		void Create(int32_t width, int32_t depth, _Ty val = 0)
		{
			auto	dv = div(width);
			if (dv.rem) ++dv.quot;

			this->m_wordwidths = dv.quot;							// 64bit로 개산한 가로 개수
			this->m_bitswidths = this->m_wordwidths * __NBITMASK;	// 64bit로 개산한 가로 개수가 표현가능한 bit 개수
			this->resize(this->m_wordwidths * depth, val);			// wordwidths_ * depth = 총 64bit 개수

			this->m_width = width;
			this->m_depth = depth;
		}

		//--------------------------------------------------------------------
		// Class: Set()
		// Desc : Data를 복사합니다.
		//--------------------------------------------------------------------
		bool Set(_Ty* pData, int32_t Count)
		{
			int32_t Size = sizeof(_Ty) * Count;
			assert((int32_t)this->size() == Size);
			if ((int32_t)this->size() < Size) return false;

			std::memcpy((void*)this->data(), (void*)pData, Size);
			return true;
		}

		//--------------------------------------------------------------------
		// Class: GetValue()
		// Desc : x, y 위치의 _Ty 정보를 가져옵니다.
		//--------------------------------------------------------------------
		_Ty GetValue(int32_t x, int32_t y)
		{
			auto pos = bitsposition(x, y);
			if (pos < 0 || pos >= (static_cast<int32_t>(this->size()) * __NBITMASK))
				return ~(0);

			return (*this)[static_cast<int32_t>(pos / __NBITMASK)];
		}

		//--------------------------------------------------------------------
		// Class: Clear()
		// Desc : Data를 모두 0 으로 초기화합니다.
		//--------------------------------------------------------------------
		void Clear(_Ty val = 0)
		{
			std::fill(this->begin(), this->end(), val);
		}

		//--------------------------------------------------------------------
		// Class: SetAt()
		// Desc : x, y 위치 Bit를 1 으로 설정합니다.
		//--------------------------------------------------------------------
		bool SetAt(int32_t x, int32_t y, bool flags)
		{
			auto pos = bitsposition(x, y);
			if (pos < 0 || pos >= (static_cast<int32_t>(this->size()) * __NBITMASK))
				return false;

			auto dv = div(pos);
			if (flags)
				(*this)[dv.quot] |= divmaskbits(dv);
			else
				(*this)[dv.quot] &= ~divmaskbits(dv);
			return true;
		}

		//--------------------------------------------------------------------
		// Class: IsSet()
		// Desc : x, y 위치 Bit가 On인지 확인합니다.
		//--------------------------------------------------------------------
		bool IsSet(int32_t x, int32_t y)
		{
			auto pos = bitsposition(x, y);
			if (pos < 0 || pos >= (static_cast<int32_t>(this->size()) * __NBITMASK))
				return true;

			auto dv = div(pos);
			return ((*this)[dv.quot] & divmaskbits(dv)) ? true : false;
		}

	};


	//--------------------------------------------------------------------
	// Class: CollisionMap
	// Desc	: 
	// 
	//--------------------------------------------------------------------
	/*
		auto spCollision = std::make_shared<pathfind::CollisionMap>();
		if (!spCollision)
			throw std::bad_alloc();
	*/
	class CollisionMap
	{
		int32_t	m_width = 0;
		int32_t	m_height = 0;

		static const int64_t npos = ~(0);	//	default npos == -1

		BitArray<int64_t>	m_xBoundaryPoints;
		BitArray<int64_t>	m_yBoundaryPoints;

	public:
		typedef std::shared_ptr<CollisionMap>		SharedPtr;
		typedef std::weak_ptr<CollisionMap>			WeakPtr;

		virtual	~CollisionMap() {};
		bool	Create(const int32_t& width, const int32_t& height);
		CollisionMap& operator = (const CollisionMap& B);

		//	[일반함수]
		auto	GetWidth() const ->decltype(m_width) { return m_width; }
		auto	GetHeight() const->decltype(m_height) { return m_height; }
		void	SetWidth(const decltype(m_width)& o) { m_width = o; }
		void	SetHeight(const decltype(m_height)& o) { m_height = o; }
		bool	IsOutBound(int32_t x, int32_t y);
		bool	IsCollision(int32_t x, int32_t y);

		//	[충돌맵]
		void	SetAt(int32_t x, int32_t y);
		void	ClrAt(int32_t x, int32_t y);

		// [길찾기 가속용]  : BLJPS 실시간 검색을 위한 함수
		int32_t	GetCloseValue(int32_t x, int32_t y, bool xaxis, bool forward);
		int32_t	GetOpenValue(int32_t x, int32_t y, bool xaxis, bool forward);

	private:
		int32_t	_GetPosX(int32_t x, int32_t y);
		int32_t	_GetPosY(int32_t x, int32_t y);

		static bool _flsll(unsigned long& index, uint64_t word);
		static bool _ffsll(unsigned long& index, uint64_t word);
	};




	//--------------------------------------------------------------------
	// Class: Coord 
	// Desc : TILE COORDINATE
	//--------------------------------------------------------------------
	struct Coord
	{
		int32_t m_x = -1;
		int32_t m_y = -1;

	public:
		Coord() = default;
		Coord(int32_t _x, int32_t _y) : m_x(_x), m_y(_y) { }

		void Add(const Coord& rhs) { m_x += rhs.m_x;	m_y += rhs.m_y; }
		void Clear() { m_x = -1;	m_y = -1; }
		bool IsEmpty() { return (m_x == -1 && m_y == -1); }

		bool     operator ==(const Coord& C) const { return ((C.m_x == m_x) && (C.m_y == m_y)); }
		bool     operator !=(const Coord& C) const { return ((C.m_x != m_x) || (C.m_y != m_y)); }

		float DistanceSqrt(const Coord& rhs);
	};

	//--------------------------------------------------------------------
	// Class: Node
	// Desc : A star 길찾기 방식에 사용되어지는 정보를 포함하는 Node
	//--------------------------------------------------------------------
	struct Node
	{
		typedef std::shared_ptr<Node>	SharedPtr;
		typedef std::weak_ptr<Node>		WeakPtr;

		Node::SharedPtr	m_spParent = nullptr;
		Coord			m_Pos;					// Compare Same Position
		char				m_CardinalDir = 0;
		float				m_Score = 0;
		float				m_Heuri = 0;
		float				m_Total = 0;			// Total Score가 적은 Node 가 P.Queue 에서 위로 올라간다.

		Node() = default;

		void Clear();
		void Set(Node::SharedPtr spParent, const Coord& Pos, const Coord& End, const char CardinalDir);

	};


	//--------------------------------------------------------------------
	// class: PriorityQueue 
	// Desc : priority queue
	//--------------------------------------------------------------------
	class PriorityQueue
	{
		std::vector<Node::SharedPtr> m_Heap;

	public:
		PriorityQueue() = default;
		~PriorityQueue() = default;

		void InsertNewone(Node::SharedPtr value)	{ m_Heap.push_back(value); _ShiftUp(m_Heap.size() - 1); }
		bool InsertSmaller(Node::SharedPtr value);

		Node::SharedPtr PopMax();

		const	Node::SharedPtr Max() const	{ return m_Heap.front(); }
		int32_t	Count() const					{ return (int32_t)m_Heap.size(); }
		void	Clear()							{ m_Heap.clear(); }

	private:
		typedef std::vector<Node::SharedPtr>::size_type _heap_size_t;

		void _BuildHeap();
		void _ShiftDown(_heap_size_t parent);
		void _ShiftUp(_heap_size_t node);
	};




	//--------------------------------------------------------------------
	// Class: Path
	// Desc : JPS (B)
	//--------------------------------------------------------------------
	class Path
	{
	private:
		//Directions
		// N, NE, E, SE, S, SW, W, NW , StartPosition
		const int32_t					NO_DIRECTION = 8;

		// Priority Queue (Opened List)
		PriorityQueue						m_OpenList;

		// searched (Closed List)
		BitArray<int64_t>				m_Searched;

		// Goal node 
		Coord					m_EndPos;

		//-------- Field Collision 에서 얻어오는 함수 -----------
		CollisionMap::SharedPtr		m_fiieldCollision;

		int32_t						m_GridWidth = 0, m_GridHeight = 0;
		//------------------------------------------------------ 

	public:
		Path() = default;
		~Path() = default;

		void Init(CollisionMap::SharedPtr fiieldCollision);
		void Destroy();

		bool Search(int32_t _sX, int32_t _sY, int32_t _eX, int32_t _eY, std::list<Coord>& ResultNodes);

	private:
		//--------------------------------------------------------------------
		// Code	: IsPassable()
		// Desc	: 해당 좌표가 이동가능한지 검사
		//--------------------------------------------------------------------
		inline bool _IsPassable(const Coord& c) { return !m_fiieldCollision->IsCollision(c.m_x, c.m_y); }

		//--------------------------------------------------------------------
		// Code	: GetNorthEndPointReOpenBB()
		// Desc	: EndPointReOpen 을 pair 로 얻어오는 함수들
		//--------------------------------------------------------------------
		std::pair<int32_t, int32_t> _GetNorthEndPointReOpenBB(int32_t x, int32_t y);
		std::pair<int32_t, int32_t> _GetSouthEndPointReOpenBB(int32_t x, int32_t y);
		std::pair<int32_t, int32_t> _GetEastEndPointReOpenBB(int32_t x, int32_t y);
		std::pair<int32_t, int32_t> _GetWestEndPointReOpenBB(int32_t x, int32_t y);

		//--------------------------------------------------------------------
		Coord _NextCoordinate(const Coord& c, const int32_t dir);
		int32_t _GetCoordinateDir(const Coord& S, const Coord& D);

		inline int32_t _DirIsDiagonal(const int32_t dir) { return (dir % 2) != 0; }
		inline int32_t _Implies(const int32_t a, const int32_t b) { return a ? b : 1; }
		inline int32_t _AddDirectionToSet(const int32_t dirs, const int32_t dir) { return dirs | 1 << dir; }
		int32_t _ForcedNeighbours(const Coord& coord, const int32_t dir);
		int32_t _NaturalNeighbours(const int32_t dir);
		bool _GetJumpPointNew(Coord s, const char direction, Coord& jp);
		Coord _JumpNew(const Coord& c, const char dir);

	public:
		//--------------------------------------------------------------------
		// Code	: PullingString()
		// Desc	: Straighten JigZag Coordinates 
		//--------------------------------------------------------------------
		bool PullingString(std::list<Coord>& ResultNodes);
		bool IsStraightPassable(int32_t fromX, int32_t fromY, int32_t toX, int32_t toY);
	};



} // jpspath

