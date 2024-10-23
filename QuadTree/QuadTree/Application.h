#pragma once
#include<memory>
#include<vector>
class IDivider;
class Actor;
class Application
{
private:
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;
	std::shared_ptr<IDivider> rootDivider_;
	std::vector<std::shared_ptr<Actor>> actors_;

	void TraverseActors(std::shared_ptr<IDivider> divider, int limit = 5);

public:
	static Application& GetInstance();
	bool Init();
	void Run();
	void Terminate();
};

