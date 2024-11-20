#include <algorithm>	

#include "JPSPathFinder.h"

namespace jpspath {




	template < class _Ty >
	bool InBoundsEx(_Ty const& v, _Ty Min, _Ty Max)
	{
		if (Min > Max) { _Ty T = Min; Min = Max; Max = T; }
		return v<Min ? false : v>Max ? false : true;
	}

	//--------------------------------------------------------------------
	// Code : Create
	// Desc : Collision 생성
	//--------------------------------------------------------------------
	bool CollisionMap::Create(const int32_t& width, const int32_t& height)
	{
		this->m_width = width;
		this->m_height = height;

		m_xBoundaryPoints.Create(width, height);
		m_yBoundaryPoints.Create(height, width);

		// OUT OF BOUND COLLISION SET
		for (int x = width; x < m_xBoundaryPoints.GetBitsWidths(); ++x)
		{
			for (int y = height; y < m_yBoundaryPoints.GetBitsWidths(); ++y)
			{
				SetAt(x, y);
			}
		}

		return true;
	}

	//--------------------------------------------------------------------
	// Code : operator = 
	// Desc : Collision 복사
	//--------------------------------------------------------------------
	CollisionMap& CollisionMap::operator = (const CollisionMap& B)
	{
		this->m_width = B.m_width;
		this->m_height = B.m_height;

		this->m_xBoundaryPoints = B.m_xBoundaryPoints;
		this->m_yBoundaryPoints = B.m_yBoundaryPoints;

		return *this;
	}

	//--------------------------------------------------------------------
	// Code : IsOutBound
	// Desc : x, y 좌표가 크기를 벗어나는지 
	//--------------------------------------------------------------------
	bool CollisionMap::IsOutBound(int32_t x, int32_t y)
	{
		if (x < 0)			return true;
		if (y < 0)			return true;
		if (x >= m_width)	return true;
		if (y >= m_height)	return true;
		return false;
	}

	//--------------------------------------------------------------------
	// Code : IsCollision
	// Desc : x, y Collision 검사
	//--------------------------------------------------------------------
	bool CollisionMap::IsCollision(int32_t x, int32_t y)
	{
		return m_xBoundaryPoints.IsSet(x, y);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//	[충돌맵]
	//--------------------------------------------------------------------
	// Code : SetAt
	// Desc : x, y 좌표를 충돌지점으로 설정
	//--------------------------------------------------------------------
	void CollisionMap::SetAt(int32_t x, int32_t y)
	{
		m_xBoundaryPoints.SetAt(x, y, true);
		m_yBoundaryPoints.SetAt(y, x, true);
	}

	//--------------------------------------------------------------------
	// Code : ClrAt
	// Desc : x, y 좌표를 충돌지점에서 해제
	//--------------------------------------------------------------------
	void CollisionMap::ClrAt(int32_t x, int32_t y)
	{
		m_xBoundaryPoints.SetAt(x, y, false);
		m_yBoundaryPoints.SetAt(y, x, false);
	}

	// [길찾기 가속용]  : BLJPS 실시간 검색을 위한 함수
	//--------------------------------------------------------------------
	// Code : GetCloseValue
	// Desc : x, y 좌표에서 x or y 축 forward(+) or backward(-) 방향으로 검색
	//--------------------------------------------------------------------
	int32_t  CollisionMap::GetCloseValue(int32_t x, int32_t y, bool xaxis, bool forward)
	{
		static const uint64_t plusTable[] = {
			18446744073709551615ULL,		18446744073709551614ULL,		18446744073709551612ULL,		18446744073709551608ULL,
			18446744073709551600ULL,		18446744073709551584ULL,		18446744073709551552ULL,		18446744073709551488ULL,
			18446744073709551360ULL,		18446744073709551104ULL,		18446744073709550592ULL,		18446744073709549568ULL,
			18446744073709547520ULL,		18446744073709543424ULL,		18446744073709535232ULL,		18446744073709518848ULL,
			18446744073709486080ULL,		18446744073709420544ULL,		18446744073709289472ULL,		18446744073709027328ULL,
			18446744073708503040ULL,		18446744073707454464ULL,		18446744073705357312ULL,		18446744073701163008ULL,
			18446744073692774400ULL,		18446744073675997184ULL,		18446744073642442752ULL,		18446744073575333888ULL,
			18446744073441116160ULL,		18446744073172680704ULL,		18446744072635809792ULL,		18446744071562067968ULL,
			18446744069414584320ULL,		18446744065119617024ULL,		18446744056529682432ULL,		18446744039349813248ULL,
			18446744004990074880ULL,		18446743936270598144ULL,		18446743798831644672ULL,		18446743523953737728ULL,
			18446742974197923840ULL,		18446741874686296064ULL,		18446739675663040512ULL,		18446735277616529408ULL,
			18446726481523507200ULL,		18446708889337462784ULL,		18446673704965373952ULL,		18446603336221196288ULL,
			18446462598732840960ULL,		18446181123756130304ULL,		18445618173802708992ULL,		18444492273895866368ULL,
			18442240474082181120ULL,		18437736874454810624ULL,		18428729675200069632ULL,		18410715276690587648ULL,
			18374686479671623680ULL,		18302628885633695744ULL,		18158513697557839872ULL,		17870283321406128128ULL,
			17293822569102704640ULL,		16140901064495857664ULL,		13835058055282163712ULL,		9223372036854775808ULL,
		};

		static const uint64_t minusTable[] = {
			1ULL,						3ULL,						7ULL,						15ULL,
			31ULL,						63ULL,						127ULL,						255ULL,
			511ULL,						1023ULL,					2047ULL,					4095ULL,
			8191ULL,					16383ULL,					32767ULL,					65535ULL,
			131071ULL,					262143ULL,					524287ULL,					1048575ULL,
			2097151ULL,					4194303ULL,					8388607ULL,					16777215ULL,
			33554431ULL,				67108863ULL,				134217727ULL,				268435455ULL,
			536870911ULL,				1073741823ULL,				2147483647ULL,				4294967295ULL,
			8589934591ULL,				17179869183ULL,				34359738367ULL,				68719476735ULL,
			137438953471ULL,			274877906943ULL,			549755813887ULL,			1099511627775ULL,
			2199023255551ULL,			4398046511103ULL,			8796093022207ULL,			17592186044415ULL,
			35184372088831ULL,			70368744177663ULL,			140737488355327ULL,			281474976710655ULL,
			562949953421311ULL,			1125899906842623ULL,		2251799813685247ULL,		4503599627370495ULL,
			9007199254740991ULL,		18014398509481983ULL,		36028797018963967ULL,		72057594037927935ULL,
			144115188075855871ULL,		288230376151711743ULL,		576460752303423487ULL,		1152921504606846975ULL,
			2305843009213693951ULL,		4611686018427387903ULL,		9223372036854775807ULL,		18446744073709551615ULL,
		};


		int32_t	maxvalue = xaxis ? m_width : m_height;
		auto& maskbound = xaxis ? m_xBoundaryPoints : m_yBoundaryPoints;
		auto	variable = xaxis ? x : y;
		auto	nbitmask = xaxis ? m_xBoundaryPoints.__NBITMASK : m_yBoundaryPoints.__NBITMASK;
		if (forward)
		{
			auto	MAX = maskbound.GetWordWidths();
			for (int i = 0; i < MAX; ++i)
			{
				auto  pos = xaxis ? _GetPosX(x + i * nbitmask, y) : _GetPosY(x, y + i * nbitmask);
				if (pos == npos) return maxvalue;

				//auto  value = maskbound.GetValue(pos);
				auto  value = maskbound[pos / nbitmask];
				if (i == 0) value &= plusTable[(variable % nbitmask)];

				unsigned long index = 0;
				if (CollisionMap::_ffsll(index, value))
				{
					return (variable - (variable % nbitmask) + i * nbitmask) + index;
				}
			}
			return maxvalue;
		}
		else
		{

			auto	MAX = maskbound.GetWordWidths();
			for (int i = 0; i < MAX; ++i)
			{
				auto  pos = xaxis ? _GetPosX(x - i * nbitmask, y) : _GetPosY(x, y - i * nbitmask);
				if (pos == npos) return -1;

				//auto  value = maskbound.GetValue(pos);
				auto  value = maskbound[pos / nbitmask];
				if (i == 0) value &= minusTable[(variable % nbitmask)];

				unsigned long index = 0;
				if (CollisionMap::_flsll(index, value))
				{
					return (variable - (variable % nbitmask) - i * nbitmask) + index;
				}
			}
			return -1;
		}
	}

	//--------------------------------------------------------------------
	// Code : GetOpenValue
	// Desc : x, y 좌표에서 x or y 축 forward(+) or backward(-) 방향으로 검색
	//--------------------------------------------------------------------
	int32_t CollisionMap::GetOpenValue(int32_t x, int32_t y, bool xaxis, bool forward)
	{
		const static uint64_t plusTable[] = {
			18446744073709551615ULL,		18446744073709551614ULL,		18446744073709551612ULL,		18446744073709551608ULL,
			18446744073709551600ULL,		18446744073709551584ULL,		18446744073709551552ULL,		18446744073709551488ULL,
			18446744073709551360ULL,		18446744073709551104ULL,		18446744073709550592ULL,		18446744073709549568ULL,
			18446744073709547520ULL,		18446744073709543424ULL,		18446744073709535232ULL,		18446744073709518848ULL,
			18446744073709486080ULL,		18446744073709420544ULL,		18446744073709289472ULL,		18446744073709027328ULL,
			18446744073708503040ULL,		18446744073707454464ULL,		18446744073705357312ULL,		18446744073701163008ULL,
			18446744073692774400ULL,		18446744073675997184ULL,		18446744073642442752ULL,		18446744073575333888ULL,
			18446744073441116160ULL,		18446744073172680704ULL,		18446744072635809792ULL,		18446744071562067968ULL,
			18446744069414584320ULL,		18446744065119617024ULL,		18446744056529682432ULL,		18446744039349813248ULL,
			18446744004990074880ULL,		18446743936270598144ULL,		18446743798831644672ULL,		18446743523953737728ULL,
			18446742974197923840ULL,		18446741874686296064ULL,		18446739675663040512ULL,		18446735277616529408ULL,
			18446726481523507200ULL,		18446708889337462784ULL,		18446673704965373952ULL,		18446603336221196288ULL,
			18446462598732840960ULL,		18446181123756130304ULL,		18445618173802708992ULL,		18444492273895866368ULL,
			18442240474082181120ULL,		18437736874454810624ULL,		18428729675200069632ULL,		18410715276690587648ULL,
			18374686479671623680ULL,		18302628885633695744ULL,		18158513697557839872ULL,		17870283321406128128ULL,
			17293822569102704640ULL,		16140901064495857664ULL,		13835058055282163712ULL,		9223372036854775808ULL,
		};

		const static uint64_t minusTable[] = {
			1ULL,						3ULL,						7ULL,						15ULL,
			31ULL,						63ULL,						127ULL,						255ULL,
			511ULL,						1023ULL,					2047ULL,					4095ULL,
			8191ULL,					16383ULL,					32767ULL,					65535ULL,
			131071ULL,					262143ULL,					524287ULL,					1048575ULL,
			2097151ULL,					4194303ULL,					8388607ULL,					16777215ULL,
			33554431ULL,				67108863ULL,				134217727ULL,				268435455ULL,
			536870911ULL,				1073741823ULL,				2147483647ULL,				4294967295ULL,
			8589934591ULL,				17179869183ULL,				34359738367ULL,				68719476735ULL,
			137438953471ULL,			274877906943ULL,			549755813887ULL,			1099511627775ULL,
			2199023255551ULL,			4398046511103ULL,			8796093022207ULL,			17592186044415ULL,
			35184372088831ULL,			70368744177663ULL,			140737488355327ULL,			281474976710655ULL,
			562949953421311ULL,			1125899906842623ULL,		2251799813685247ULL,		4503599627370495ULL,
			9007199254740991ULL,		18014398509481983ULL,		36028797018963967ULL,		72057594037927935ULL,
			144115188075855871ULL,		288230376151711743ULL,		576460752303423487ULL,		1152921504606846975ULL,
			2305843009213693951ULL,		4611686018427387903ULL,		9223372036854775807ULL,		18446744073709551615ULL,
		};


		int32_t	maxvalue = xaxis ? m_width : m_height;
		auto& maskbound = xaxis ? m_xBoundaryPoints : m_yBoundaryPoints;
		auto	variable = xaxis ? x : y;
		auto	nbitmask = xaxis ? m_xBoundaryPoints.__NBITMASK : m_yBoundaryPoints.__NBITMASK;
		if (forward)
		{
			auto	MAX = maskbound.GetWordWidths();
			for (int i = 0; i < MAX; ++i)
			{
				auto  pos = xaxis ? _GetPosX(x + i * nbitmask, y) : _GetPosY(x, y + i * nbitmask);
				if (pos == npos) return maxvalue;

				//auto  value = ~maskbound.GetValue(pos);
				auto  value = ~maskbound[pos / nbitmask];
				if (i == 0) value &= plusTable[(variable % nbitmask)];

				unsigned long index = 0;
				if (CollisionMap::_ffsll(index, value))
				{
					return (variable - (variable % nbitmask) + i * nbitmask) + index;
				}
			}
			return maxvalue;
		}
		else
		{
			auto	MAX = maskbound.GetWordWidths();
			for (int i = 0; i < MAX; ++i)
			{
				auto  pos = xaxis ? _GetPosX(x - i * nbitmask, y) : _GetPosY(x, y - i * nbitmask);
				if (pos == npos) return -1;

				//auto  value = ~maskbound.GetValue(pos);
				auto  value = ~maskbound[pos / nbitmask];
				if (i == 0) value &= minusTable[(variable % nbitmask)];

				unsigned long index = 0;
				if (CollisionMap::_flsll(index, value))
				{
					return (variable - (variable % nbitmask) - i * nbitmask) + index;
				}
			}
			return -1;
		}

	}

	int32_t CollisionMap::_GetPosX(int32_t x, int32_t y)
	{
		if (x < 0 || x >= m_width || y < 0 || y >= m_height) return npos;
		return x + y * m_xBoundaryPoints.GetBitsWidths();
	}

	int32_t CollisionMap::_GetPosY(int32_t x, int32_t y)
	{
		if (x < 0 || x >= m_width || y < 0 || y >= m_height) return npos;
		return x * m_yBoundaryPoints.GetBitsWidths() + y;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//--------------------------------------------------------------------
	// Code : _flsll
	// Desc : _BitScanReverse64 과 동일한 코드 (Windows, Android, IOS 모두 동일한 코드를 위해)
	//--------------------------------------------------------------------
	bool CollisionMap::_flsll(unsigned long& index, uint64_t word)
	{
		unsigned long num = 63;
		if (!word)
			return false;

		if (!(word & 0xffffffff00000000ULL)) {
			word <<= 32;
			num -= 32;
		}
		if (!(word & 0xffff000000000000ULL)) {
			word <<= 16;
			num -= 16;
		}
		if (!(word & 0xff00000000000000ULL)) {
			word <<= 8;
			num -= 8;
		}
		if (!(word & 0xf000000000000000ULL)) {
			word <<= 4;
			num -= 4;
		}
		if (!(word & 0xc000000000000000ULL)) {
			word <<= 2;
			num -= 2;
		}
		if (!(word & 0x8000000000000000ULL)) {
			word <<= 1;
			num -= 1;
		}
		index = num;
		return true;
	}

	//--------------------------------------------------------------------
	// Code : _ffsll
	// Desc : _BitScanForward64 과 동일한 코드 (Windows, Android, IOS 모두 동일한 코드를 위해)
	//--------------------------------------------------------------------
	bool CollisionMap::_ffsll(unsigned long& index, uint64_t word)
	{
		unsigned long num = 0;
		if (!word)
			return false;
		if ((word & 0xffffffffULL) == 0) {
			num += 32;
			word >>= 32;
		}
		if ((word & 0xffffULL) == 0) {
			num += 16;
			word >>= 16;
		}
		if ((word & 0xffULL) == 0) {
			num += 8;
			word >>= 8;
		}
		if ((word & 0xfULL) == 0) {
			num += 4;
			word >>= 4;
		}
		if ((word & 0x3ULL) == 0) {
			num += 2;
			word >>= 2;
		}
		if ((word & 0x1ULL) == 0)
			num += 1;

		index = num;
		return true;
	}



	void Path::Init(CollisionMap::SharedPtr fiieldCollision)
	{
		m_fiieldCollision = fiieldCollision;
		if (nullptr == m_fiieldCollision) return;

		m_GridWidth = fiieldCollision->GetWidth();
		m_GridHeight = fiieldCollision->GetHeight();
		m_Searched.Create(m_GridWidth, m_GridHeight);
	}

	void Path::Destroy()
	{
		m_fiieldCollision = nullptr;
		m_GridWidth = 0;
		m_GridHeight = 0;
		m_Searched.Clear();
	}

	//--------------------------------------------------------------------
	// Class: Search()
	// Desc	: 
	//--------------------------------------------------------------------
	bool Path::Search(int32_t _sX, int32_t _sY, int32_t _eX, int32_t _eY, std::list<Coord>& ResultNodes)
	{
		//===================================================================
		// CHECK
		//===================================================================

		if ((_sX < 0 || _sX >= m_GridWidth) ||
			(_sY < 0 || _sY >= m_GridHeight) ||
			(_sX == _eX && _sY == _eY) ||
			(m_fiieldCollision == nullptr))
		{
			return false;
		}

		//===================================================================
		// PREPARE
		//===================================================================
		m_EndPos.m_x = _eX;
		m_EndPos.m_y = _eY;
		ResultNodes.clear();
		m_OpenList.Clear();	// Ready the openlist, closed list and node container for search
		m_Searched.Clear();

		//================================================
		// START  : Insert the start nodes into the openList
		//================================================
		auto spStartNode = std::make_shared<Node>();
		if (!spStartNode)
			throw std::bad_alloc();
		spStartNode->Set(nullptr, Coord(_sX, _sY), m_EndPos, 8);
		//================================================

		m_OpenList.InsertNewone(spStartNode);
		m_Searched.SetAt(spStartNode->m_Pos.m_x, spStartNode->m_Pos.m_y, true);

		//===================================================================
		// LOOP
		//===================================================================
		// Keep iterating over openlist until a solution is found or list is empty
		while (m_OpenList.Count())
		{
			auto  spCurrNode = m_OpenList.PopMax();
			int32_t Directions = _ForcedNeighbours(spCurrNode->m_Pos, spCurrNode->m_CardinalDir) | _NaturalNeighbours(spCurrNode->m_CardinalDir);

			for (int32_t dir = 0; dir < 8; dir++)
			{
				if ((1 << dir) & Directions)
				{
					auto CoordinateNewC = _JumpNew(spCurrNode->m_Pos, dir);
					if (!CoordinateNewC.IsEmpty())
					{
						//================================================
						// ARRIVED
						//================================================
						if (CoordinateNewC == m_EndPos)
						{
							ResultNodes.push_front(m_EndPos);

							// RECURSIVE TO START POINT
							auto  spTraceNode = spCurrNode;
							int32_t CurDir = _GetCoordinateDir(m_EndPos, spCurrNode->m_Pos);
							while (spTraceNode)
							{
								int32_t NextDir = 0;
								if (spTraceNode->m_spParent)	NextDir = _GetCoordinateDir(spTraceNode->m_Pos, spTraceNode->m_spParent->m_Pos);
								if (CurDir != NextDir)			ResultNodes.push_front(spTraceNode->m_Pos);
								spTraceNode = spTraceNode->m_spParent;
								CurDir = NextDir;
							}
							return true;
						}

						//================================================
						// KEEP SEARCH
						//================================================
						auto newNode = std::make_shared<Node>();
						if (!newNode)
							throw std::bad_alloc();
						newNode->Set(spCurrNode, CoordinateNewC, m_EndPos, dir);
						//================================================

						// Insert or update node into the openList
						if (!m_Searched.IsSet(CoordinateNewC.m_x, CoordinateNewC.m_y))
						{
							m_OpenList.InsertNewone(newNode);
							m_Searched.SetAt(CoordinateNewC.m_x, CoordinateNewC.m_y, true);
						}
						else
						{
							m_OpenList.InsertSmaller(newNode);
						}
					}
				}
			}
		}

		return false;
	}

	//--------------------------------------------------------------------
	// Code	: GetNorthEndPointReOpenBB()
	// Desc	: EndPointReOpen 을 pair 로 얻어오는 함수들
	//--------------------------------------------------------------------

	std::pair<int32_t, int32_t> Path::_GetNorthEndPointReOpenBB(int32_t x, int32_t y)
	{
		if (x < 0 || x >= m_GridWidth)
			return std::pair<int32_t, int32_t>(-1, -1);

		if (m_fiieldCollision->IsCollision(x, y))
		{
			// 충돌지점에 있으면 다음 오픈지점만 돌려줍니다.
			auto openpos = m_fiieldCollision->GetOpenValue(x, y, false, false);
			return std::pair<int32_t, int32_t>(openpos, openpos);
		}
		else
		{
			// 오픈지점의 마지막 위치와 다음 오픈지점을 돌려줍니다.
			auto closepos = m_fiieldCollision->GetCloseValue(x, y, false, false);
			auto openpos = m_fiieldCollision->GetOpenValue(x, closepos, false, false);
			return std::pair<int32_t, int32_t>(closepos + 1, openpos);
		}
	}

	std::pair<int32_t, int32_t> Path::_GetSouthEndPointReOpenBB(int32_t x, int32_t y)
	{
		if (x < 0 || x >= m_GridWidth)
			return std::pair<int32_t, int32_t>(m_GridHeight, m_GridHeight);

		if (m_fiieldCollision->IsCollision(x, y))
		{
			// 충돌지점에 있으면 다음 오픈지점만 돌려줍니다.
			auto openpos = m_fiieldCollision->GetOpenValue(x, y, false, true);
			return std::pair<int32_t, int32_t>(openpos, openpos);
		}
		else
		{
			// 오픈지점의 마지막 위치와 다음 오픈지점을 돌려줍니다.
			auto closepos = m_fiieldCollision->GetCloseValue(x, y, false, true);
			auto openpos = m_fiieldCollision->GetOpenValue(x, closepos, false, true);
			return std::pair<int32_t, int32_t>(closepos - 1, openpos);
		}
	}


	std::pair<int32_t, int32_t> Path::_GetEastEndPointReOpenBB(int32_t x, int32_t y)
	{
		if (y < 0 || y >= m_GridHeight)
			return std::pair<int32_t, int32_t>(m_GridWidth, m_GridWidth);

		if (m_fiieldCollision->IsCollision(x, y))
		{
			// 충돌지점에 있으면 다음 오픈지점만 돌려줍니다.
			auto openpos = m_fiieldCollision->GetOpenValue(x, y, true, true);
			return std::pair<int32_t, int32_t>(openpos, openpos);
		}
		else
		{
			// 오픈지점의 마지막 위치와 다음 오픈지점을 돌려줍니다.
			auto closepos = m_fiieldCollision->GetCloseValue(x, y, true, true);
			auto openpos = m_fiieldCollision->GetOpenValue(closepos, y, true, true);
			return std::pair<int32_t, int32_t>(closepos - 1, openpos);
		}
	}

	//--------------------------------------------------------------------
	// Code	: PullingString()
	// Desc	: Straighten JigZag Coordinates 
	//--------------------------------------------------------------------
	std::pair<int32_t, int32_t> Path::_GetWestEndPointReOpenBB(int32_t x, int32_t y)
	{
		if (y < 0 || y >= m_GridHeight)
			return std::pair<int32_t, int32_t>(-1, -1);

		if (m_fiieldCollision->IsCollision(x, y))
		{
			// 충돌지점에 있으면 다음 오픈지점만 돌려줍니다.
			auto openpos = m_fiieldCollision->GetOpenValue(x, y, true, false);
			return std::pair<int32_t, int32_t>(openpos, openpos);
		}
		else
		{
			// 오픈지점의 마지막 위치와 다음 오픈지점을 돌려줍니다.
			auto closepos = m_fiieldCollision->GetCloseValue(x, y, true, false);
			auto openpos = m_fiieldCollision->GetOpenValue(closepos, y, true, false);
			return std::pair<int32_t, int32_t>(closepos + 1, openpos);
		}
	}


	//--------------------------------------------------------------------

	Coord Path::_NextCoordinate(const Coord& c, const int32_t dir)
	{
		static int32_t dirMov[] = { 0,-1,1,-1,1,0,1,1,0,1,-1,1,-1,0,-1,-1,0,0 };
		return Coord(c.m_x + dirMov[dir * 2], c.m_y + dirMov[dir * 2 + 1]);
	}

	int32_t Path::_GetCoordinateDir(const Coord& S, const Coord& D)
	{
		int32_t dirs = 0;
		if (S.m_x > D.m_x)  dirs += 1 << 4;
		else if (S.m_x < D.m_x)	 dirs += 1 << 2;
		if (S.m_y > D.m_y)  dirs += 1 << 3;
		else if (S.m_y < D.m_y)	 dirs += 1 << 1;
		return dirs;
	}

	int32_t Path::_ForcedNeighbours(const Coord& coord, const int32_t dir)
	{
		if (dir > 7)
			return 0;

		int32_t dirs = 0;
#define ENTERABLE(n) _IsPassable ( _NextCoordinate (coord, (dir + (n)) % 8))

		if (_DirIsDiagonal(dir)) {
			if (!_Implies(ENTERABLE(6), ENTERABLE(5)))
				dirs = _AddDirectionToSet(dirs, (dir + 6) % 8);
			if (!_Implies(ENTERABLE(2), ENTERABLE(3)))
				dirs = _AddDirectionToSet(dirs, (dir + 2) % 8);

		}
		else {
			if (!_Implies(ENTERABLE(7), ENTERABLE(6)))
				dirs = _AddDirectionToSet(dirs, (dir + 7) % 8);
			if (!_Implies(ENTERABLE(1), ENTERABLE(2)))
				dirs = _AddDirectionToSet(dirs, (dir + 1) % 8);
		}

#undef ENTERABLE	

		return dirs;
	}

	int32_t Path::_NaturalNeighbours(const int32_t dir)
	{
		if (dir == NO_DIRECTION)
			return 255;

		int32_t dirs = 0;
		dirs = _AddDirectionToSet(dirs, dir);
		if (_DirIsDiagonal(dir)) {
			dirs = _AddDirectionToSet(dirs, (dir + 1) % 8);
			dirs = _AddDirectionToSet(dirs, (dir + 7) % 8);
		}
		return dirs;
	}

	bool Path::_GetJumpPointNew(Coord s, const char direction, Coord& jp)
	{
		s = _NextCoordinate(s, direction);

		if (!_IsPassable(s))
			return false;
		bool ret = false;
		std::pair<int32_t, int32_t> up, center, down;
		switch (direction)
		{

		case 0://North
			up = _GetNorthEndPointReOpenBB(s.m_x - 1, s.m_y);
			center = _GetNorthEndPointReOpenBB(s.m_x, s.m_y);
			down = _GetNorthEndPointReOpenBB(s.m_x + 1, s.m_y);

			if (s.m_x == m_EndPos.m_x && s.m_y >= m_EndPos.m_y && center.first <= m_EndPos.m_y)
			{
				jp = m_EndPos;
				return true;
			}
			if (down.first != -1 && ((down.second > -1 && down.first > center.first && down.second + 2 > center.first) || (down.first == down.second && down.first + 2 > center.first)))
			{
				jp = Coord(s.m_x, down.second + 1);
				ret = true;
			}
			if (up.first != -1 && ((up.second > -1 && up.first > center.first && up.second + 2 > center.first) || (up.first == up.second && up.first + 2 > center.first)))
			{
				jp = Coord(s.m_x, ret ? std::max(jp.m_y, up.second + 1) : up.second + 1);
				return true;
			}
			return ret;
		case 2://EAST
			up = _GetEastEndPointReOpenBB(s.m_x, s.m_y - 1);
			center = _GetEastEndPointReOpenBB(s.m_x, s.m_y);
			down = _GetEastEndPointReOpenBB(s.m_x, s.m_y + 1);

			if (s.m_y == m_EndPos.m_y && s.m_x <= m_EndPos.m_x && center.first >= m_EndPos.m_x)
			{
				jp = m_EndPos;
				return true;
			}

			if (down.first != m_GridWidth && ((down.second < m_GridWidth && down.first < center.first && down.second - 2 < center.first) || (down.first == down.second && down.first - 2 < center.first)))
			{
				jp = Coord(down.second - 1, s.m_y);
				ret = true;
			}
			if (up.first != m_GridWidth && ((up.second < m_GridWidth && up.first < center.first && up.second - 2 < center.first) || (up.first == up.second && up.first - 2 < center.first)))
			{
				jp = Coord(ret ? std::min(jp.m_x, up.second - 1) : up.second - 1, s.m_y);
				return true;
			}
			return ret;
		case 4://SOUTH
			up = _GetSouthEndPointReOpenBB(s.m_x - 1, s.m_y);
			center = _GetSouthEndPointReOpenBB(s.m_x, s.m_y);
			down = _GetSouthEndPointReOpenBB(s.m_x + 1, s.m_y);

			if (s.m_x == m_EndPos.m_x && s.m_y <= m_EndPos.m_y && center.first >= m_EndPos.m_y)
			{
				jp = m_EndPos;
				return true;
			}
			if (down.first != m_GridHeight && ((down.second < m_GridHeight && down.first < center.first && down.second - 2 < center.first) || (down.first == down.second && down.first - 2 < center.first)))
			{
				jp = Coord(s.m_x, down.second - 1);
				ret = true;
			}
			if (up.first != m_GridHeight && ((up.second < m_GridHeight && up.first < center.first && up.second - 2 < center.first) || (up.first == up.second && up.first - 2 < center.first)))
			{
				jp = Coord(s.m_x, ret ? std::min(jp.m_y, up.second - 1) : up.second - 1);
				return true;
			}
			return ret;
		case 6://WEST
			up = _GetWestEndPointReOpenBB(s.m_x, s.m_y - 1);
			center = _GetWestEndPointReOpenBB(s.m_x, s.m_y);
			down = _GetWestEndPointReOpenBB(s.m_x, s.m_y + 1);

			if (s.m_y == m_EndPos.m_y && s.m_x >= m_EndPos.m_x && center.first <= m_EndPos.m_x)
			{
				jp = m_EndPos;
				return true;
			}
			if (down.first != -1 && ((down.second > -1 && down.first > center.first && down.second + 2 > center.first) || (down.first == down.second && down.first + 2 > center.first)))
			{
				jp = Coord(down.second + 1, s.m_y);
				ret = true;
			}
			if (up.first != -1 && ((up.second > -1 && up.first > center.first && up.second + 2 > center.first) || (up.first == up.second && up.first + 2 > center.first)))
			{
				jp = Coord(ret ? std::max(jp.m_x, up.second + 1) : up.second + 1, s.m_y);
				return true;
			}
			return ret;
		}
		return false;
	}

	Coord Path::_JumpNew(const Coord& c, const char dir)
	{
		Coord nc = _NextCoordinate(c, dir);
		bool isDiag = _DirIsDiagonal(dir);
		Coord offset(0, 0);
		offset = _NextCoordinate(offset, dir);

		while (1)
		{
			if (!_IsPassable(nc))
				return Coord(-1, -1);
			if (_ForcedNeighbours(nc, dir) || m_EndPos == nc)
				return nc;
			if (isDiag)
			{
				Coord newP(-1, -1);
				if (_GetJumpPointNew(nc, (dir + 7) % 8, newP))
					return nc;
				if (_GetJumpPointNew(nc, (dir + 1) % 8, newP))
					return nc;
			}
			else
			{
				Coord newP(-1, -1);
				_GetJumpPointNew(nc, dir, newP);
				return newP;
			}
			nc.Add(offset);
		}
		return Coord(-1, -1);
	}

	//--------------------------------------------------------------------
	// Code	: PullingString()
	// Desc	: Straighten JigZag Coordinates 
	//--------------------------------------------------------------------
	bool Path::PullingString(std::list<Coord>& ResultNodes)
	{
		if (ResultNodes.size() <= 2) return false;

		auto iterS = ResultNodes.begin();
		auto iterE = ResultNodes.end();

		auto iterB = iterS;	++iterS;
		auto iterP = iterS;	++iterS;

		for (; iterS != iterE; iterP = iterS, ++iterS)
		{
			auto& BaseNode = (*iterB);
			auto& PrevNode = (*iterP);
			auto& CurrNode = (*iterS);

			if (IsStraightPassable(BaseNode.m_x, BaseNode.m_y, CurrNode.m_x, CurrNode.m_y) == true)
			{
				ResultNodes.erase(iterP);
			}
			else
			{
				iterB = iterP;
			}
		}

		if (ResultNodes.size() <= 0) return false;

		return true;
	}

	bool Path::IsStraightPassable(int32_t fromX, int32_t fromY, int32_t toX, int32_t toY)
	{
		float difX = (float)(toX - fromX);
		float difY = (float)(toY - fromY);
		float step = std::max(std::abs(difX), std::abs(difY));

		float dx = difX / step;
		float dy = difY / step;

		for (int i = 0; i <= std::ceil(step); i++)
		{
			Coord  pos(fromX + int32_t(dx * (float)i), fromY + int32_t(dy * (float)i));
			if (InBoundsEx(pos.m_x, fromX, toX) == false)		continue;
			if (InBoundsEx(pos.m_y, fromY, toY) == false)		continue;

			// 해당 SPOT이 Passable 검사
			if (_IsPassable(pos) == false) return false;
		}

		return true;
	}





	float Coord::DistanceSqrt(const Coord& rhs)
	{
		int32_t absX = std::abs(m_x - rhs.m_x);
		int32_t absY = std::abs(m_y - rhs.m_y);

		int32_t diagDist = std::min(absX, absY);
		int32_t straightDist = std::max(absX, absY) - diagDist;
		return diagDist * 1.414213562373095f + straightDist;
	}

	bool PriorityQueue::InsertSmaller(Node::SharedPtr value)
	{
		auto	pSeg = m_Heap.data();
		auto	cbSize = m_Heap.size();

		for (size_t n = 0; n < cbSize; ++n, ++pSeg)
		{
			if (value->m_Pos != (*pSeg)->m_Pos)
				continue;

			if (value->m_Total >= (*pSeg)->m_Total)
			{
				return false;
			}
			else
			{
				(*pSeg) = value;
				_ShiftUp(n);
				return true;
			}
		}

		return false;
	}

	Node::SharedPtr PriorityQueue::PopMax()
	{
		auto	result = m_Heap.front();
		m_Heap.front() = m_Heap.back();
		m_Heap.pop_back();
		_ShiftDown(0);
		return result;
	}

	void PriorityQueue::_BuildHeap()
	{
		for (int64_t i = (int64_t)(m_Heap.size() / 2); i >= 0; --i)
			_ShiftDown(i);
	}

	void PriorityQueue::_ShiftDown(_heap_size_t parent)
	{
		auto	pSeg = m_Heap.data();
		auto	cbSize = m_Heap.size();
		for (;;)
		{
			auto	left = static_cast<_heap_size_t>(parent * 2 + 1);
			// no children so done
			if (left >= cbSize) break;

			auto	right = left + 1;
			// if there is a right child, and it is smaller than left child, use the left instead
			if (right < cbSize && pSeg[right]->m_Total < pSeg[left]->m_Total)
				left = right;

			// parent is smaller than (or equal to) smallest child so done
			if (pSeg[parent]->m_Total < pSeg[left]->m_Total)
				break;

			std::swap(pSeg[parent], pSeg[left]);
			parent = left;
		}
	}

	void PriorityQueue::_ShiftUp(_heap_size_t node)
	{
		auto	pSeg = m_Heap.data();
		while (node > 0)
		{
			auto	parent = static_cast<_heap_size_t>((node - 1) / 2);		//	parent index

			// child item is larger than (or equal) parent so we're done
			if (pSeg[node]->m_Total > pSeg[parent]->m_Total)
				return;

			std::swap(pSeg[node], pSeg[parent]);
			node = parent;
		}
	}

	void Node::Clear()
	{
		m_spParent = nullptr;
		m_Pos.Clear();
		m_CardinalDir = 0;
		m_Score = 0;
		m_Heuri = 0;
		m_Total = 0;
	}

	void Node::Set(Node::SharedPtr spParent, const Coord& Pos, const Coord& End, const char CardinalDir)
	{
		m_spParent = spParent;
		m_Pos = Pos;
		m_CardinalDir = CardinalDir;
		if (m_spParent)
			m_Score = m_Pos.DistanceSqrt(m_spParent->m_Pos) + m_spParent->m_Score;
		else
			m_Score = 0;
		m_Heuri = m_Pos.DistanceSqrt(End);
		m_Total = m_Score + m_Heuri;
	}

} // namespace jpspath

