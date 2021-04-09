#include <Ice/Ice.h>
#include <Clock.h>
#include <iomanip>
#include <stdexcept>

using namespace std;
using namespace Demo;

int main(int argc, char* argv[])
{
    try
    {
        Ice::CommunicatorHolder ich(argc, argv);
        auto base = ich->stringToProxy("SimplePrinter:default -p 10000");
        auto clock = Ice::checkedCast<ClockPrx>(base);
        if (!clock)
        {
            throw std::runtime_error("Invalid proxy");
        }
        int opc;
        Horario H;
        while (1)
        {
            cout << "1 - Alterar horario\n2 - Consultar horario\nSua opcao: ";
            cin >> opc;
            if (opc == 1)
            {
                int h, m, s;
                cout << "Informe o horario separado por espaco (hh mm ss): ";
                cin >> h >> m >> s;
                H.hours = h;
                H.minutes = m;
                H.seconds = s;
                try
                {
                    clock->setTime(H);
                }
                catch (exception& e)
                {
                    cout << "Ops, alteracao de horario falhou!\n" << e.what() << '\n';
                }
            }
            else if (opc == 2)
            {
                H = clock->getTime();
                cout << "O horario eh: " << setw(2) << setfill('0') << H.hours << ':' << setw(2) << setfill('0') << H.minutes << ':' << setw(2) << setfill('0') << H.seconds << '\n';
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