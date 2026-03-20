// #pragma once
// #include "Obstacles/CutterBase.h"
//
// template <typename T = ABaseCutter>
// class OnBeginOverlapEventRegistrantBase
// {
// public:
// 	OnBeginOverlapEventRegistrantBase(T* instance)
// 	{
// 		_instance = instance;
// 	}
// 	virtual ~OnBeginOverlapEventRegistrantBase() = default;
// 	
// 	typedef void (T::*OverlapFunc)(const AActor*);
// 	
// 	
// protected:
// 	T* _instance={};
// 	OverlapFunc _overlapFunc={};
// };