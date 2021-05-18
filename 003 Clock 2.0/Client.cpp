#include <Ice/Ice.h>
#include <Clock.h>
#include <iomanip>
#include <Glacier2/Glacier2.h>
#include <Glacier2/Router.h>
#include <Glacier2/Session.h>
#include <stdexcept>

using namespace std;
using namespace Demo;

int main(int argc, char* argv[])
{

    string senderConnectionStringTCP = "ClockID:tcp -p 10000";
    string senderConnectionStringUDP = "ClockID:udp -p 10000";
    string gatewayConnectionString = "DemoGlacier2/router:tcp -p 4063";

    try
    {
        Ice::CommunicatorHolder ich(argc, argv);
                        
        bool usingGateway = false;

        shared_ptr<Glacier2::SessionPrx> session;
        if (usingGateway)
        {
            shared_ptr<Glacier2::RouterPrx> router = Ice::checkedCast<Glacier2::RouterPrx>(ich->stringToProxy(gatewayConnectionString));
            ich->setDefaultRouter(router);
            session = router->createSession("", "");
        }
        auto base = ich->stringToProxy(senderConnectionStringTCP);
        auto baseUDP = ich->stringToProxy(senderConnectionStringUDP);
        auto clockTCP = Ice::checkedCast<ClockPrx>(base);
        auto clockUDP = Ice::uncheckedCast<ClockPrx>(baseUDP->ice_datagram());

        if (!clockTCP)
        {
            throw std::runtime_error("Invalid proxy");
        }
        int opc;
        Horario H;
        
        while (1)
        {
            cout << "1 - Set Time (TCP)\n2 - Set Time (UDP)\n3 - Check Time (TCP)\n4 - Check Time (UDP)**\nYour choice: ";
            cin >> opc;
            if (opc == 1)
            {
                int h, m, s;
                cout << "Provide the time separated by spaces (hh mm ss): ";
                cin >> h >> m >> s;
                H.hours = h;
                H.minutes = m;
                H.seconds = s;
                try
                {
                    clockTCP->setTime(H);
                }
                catch (exception& e)
                {
                    cout << "Ops, Set Time failed!\n" << e.what() << '\n';
                }
            }
            else if (opc == 2)
            {
                int h, m, s;
                cout << "Provide the time separated by spaces (hh mm ss): ";
                cin >> h >> m >> s;
                H.hours = h;
                H.minutes = m;
                H.seconds = s;
                try
                {
                    clockUDP->setTime(H);
                }
                catch (exception& e)
                {
                    cout << "Ops, Set Time failed!\n" << e.what() << '\n';
                }
            }
            else if (opc == 3)
            {
                H = clockTCP->getTime();
                cout << "Current Time is: " << setw(2) << setfill('0') << H.hours << ':' << setw(2) << setfill('0') << H.minutes << ':' << setw(2) << setfill('0') << H.seconds << '\n';
            }
            else if (opc == 4) //This will fail because getTime returns something and this operation is being called in UDP
            {
                H = clockUDP->getTime();
                cout << "Current Time is: " << setw(2) << setfill('0') << H.hours << ':' << setw(2) << setfill('0') << H.minutes << ':' << setw(2) << setfill('0') << H.seconds << '\n';
            }
        }

    }
    catch (const std::exception& e)
    {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}