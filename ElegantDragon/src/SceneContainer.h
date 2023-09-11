#pragma once
#include <vector>
#include <set>
#include "GraphicObj.h"
#include "RectBoundElem.h"
#include "MathDrgn.h"

//Top level container for a scene
//Contains list of underlying objects and resources 
//Optional bounding block for Objects that rely on intersection checks

//TODO: MAKE THIS A SPECIAL GO! That way you can recursively group! Dynamically generate buffer data based on content beneth
class SceneContainer
{
protected:
	RectBoundElem TreeObjects; //O(LogN) location change, O(LogN) intersection (if balanced) O(n) worst case
	std::set<GraphicObj*> ListObjects; //O(1) location change, O(N) intersection
public:
	//future implementation if want to simplify structures
	/*enum class InsertHint
	{
		Normal,
		Static
	};*/
	
	/// <summary>
	/// Empty init 
	/// </summary>
	SceneContainer() {};


	/// <summary>
	/// 
	/// </summary>
	/// <param name="objIn"></param>
	/// <returns></returns>
	bool insertTree(GraphicObj& objIn);




	/// <summary>
	/// Query all Objects intersecting 
	/// </summary>
	/// <param name="queryPoint"></param>
	std::vector<GraphicObj*> SceneContainer::pointQuery(const MathVec3f queryPoint);

	//Must be called for any location change of a child
	//updateChildLocation();
};