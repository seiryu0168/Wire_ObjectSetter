#include "RootJob.h"
#include"../SceneManager.h"
#include"../SAFE_DELETE_RELEASE.h"

#pragma comment(lib, "winmm.lib")

RootJob::RootJob() 
	: GameObject(nullptr,"RootJob")
{
		Instantiate<SceneManager>(this);

}

RootJob::~RootJob()
{

}

void RootJob::Initialize()
{
}

void RootJob::Update()
{
}

void RootJob::Draw()
{

}

void RootJob::Release()
{
}