#include<Ice/Ice.h>
#include<Game.h>
#include <Glacier2/Glacier2.h>
#include <Glacier2/Router.h>
#include <Glacier2/Session.h>
#include<stdexcept>
using namespace std;
using namespace Demo;

class GameI : public Game
{
	string board[3] = { "...", "...", "..." };
	int token = 1;
	string getStatus(const Ice::Current& current) override
	{
		return board[0] + "\n" + board[1] + "\n" + board[2] + "\n";
	}

	bool play(int x, int y, const Ice::Current& current) override
	{
		if (x < 1 || x > 3 || y < 1 || y > 3 || board[x - 1][y - 1] != '.')
		{
			cout << "Invalid move!\n";
			return false;
		}
		cout << "Placed a " << token << " on coords: [" << x << ',' << y << "]\n";
		board[x - 1][y - 1] = '0' + token;
		token = !token;
		
		return true;
	}
};

int main(int argc, char* argv[])
{
	try
	{
		Ice::CommunicatorHolder ich(argc, argv);
		
		auto adapter = ich->createObjectAdapterWithEndpoints("GameAdapter", "default -p 10001");
		auto servant = make_shared<GameI>();
		
		adapter->add(servant, Ice::stringToIdentity("TicTacToe"));
		adapter->activate();
		ich->waitForShutdown();
	}
	catch (const std::exception& e)
	{
		cerr << e.what() << endl;
		return 1;
	}
	return 0;
}