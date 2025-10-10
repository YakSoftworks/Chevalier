#pragma once


#include "Core.h"

#include "ChevalierRenderer.h"

class ChevalierInstance
{

protected:

	//Called during startup to create application vulkan objects
	void InitVulkan();

	void LoopProgram();

	bool CheckShouldClose();


	ChevalierRenderer mRenderer;


public:
	void InitChevalier();

	void CleanupChevalier();

};

