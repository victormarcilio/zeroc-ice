#include<Ice/Ice.h>
#include<Game.h>
#include<memory>
#include <Glacier2/Glacier2.h>
#include <Glacier2/Router.h>
#include <Glacier2/Session.h>
#include<stdexcept>
using namespace std;
using namespace Demo;

int main(int argc, char* argv[])
{
    bool usingGateway = true;
    Ice::CommunicatorHolder ich(argc, argv);
    
    string gatewayConnectionString = "DemoGlacier2/router:tcp -p 4063 -h localhost";
    string senderConnectionString = "TicTacToe:tcp -p 10001 -h localhost";
    shared_ptr<Glacier2::SessionPrx> session;
    if (usingGateway)
    {
        shared_ptr<Glacier2::RouterPrx> router = Ice::checkedCast<Glacier2::RouterPrx>(ich->stringToProxy(gatewayConnectionString));
        ich->setDefaultRouter(router);
        session = router->createSession("", "");
    }
    try {
        auto base = ich->stringToProxy(senderConnectionString);
        auto game = Ice::checkedCast<GamePrx>(base);    
        while (1)
        {
            int x, y;
            cout << "\n\n";            
            cout << game->getStatus();           
            cout << "Provide 2 space separated integers in the interval [1-3] for your move: ";
            cin >> x >> y;            
                if (!game->play(x, y))
                    cout << "Invalid move!\n";            
        }
    }
    catch (Ice::ConnectionRefusedException e)
    {
        cout << e;
    }
    
}