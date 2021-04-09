#include <Ice/Ice.h>
#include <Clock.h>
#include <stdexcept>
#include <iomanip>
using namespace std;
using namespace Demo;

class ClockI : public Clock
{
	Horario H;
public:
	virtual void setTime(Horario h, const Ice::Current&) override;
	virtual Horario getTime(const Ice::Current&) override;
};


void ClockI::setTime(Horario h, const Ice::Current&)
{
	if (h.hours < 0 || h.hours > 23)
		throw invalid_argument("Hora invalida");
	if(h.minutes < 0 || h.minutes > 59)
		throw invalid_argument("Minuto invalido");
	if (h.seconds < 0 || h.seconds > 59)
		throw invalid_argument("Segundo invalido");

	H = h;
	cout << "Horario alterado para: " << setw(2) << setfill('0') << H.hours << ':' << setw(2) << setfill('0') << H.minutes << ':' << setw(2) << setfill('0') << H.seconds << '\n';
}

Horario ClockI::getTime(const Ice::Current&)
{
	return H;
}


int main(int argc, char* argv[])
{
	try
	{
		Ice::CommunicatorHolder ich(argc, argv);
		auto adapter = ich->createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -p 10000");
		auto servant = make_shared<ClockI>();
		adapter->add(servant, Ice::stringToIdentity("SimplePrinter"));
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
