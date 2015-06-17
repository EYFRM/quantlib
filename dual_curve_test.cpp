#include <ql/quantlib.hpp>
#include <boost/timer.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h> //atof

using namespace std;
using namespace QuantLib;

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {
	Integer sessionId() { return 0; }
}
#endif

// A structure to hold all the market quotes for a single curve together
struct CurveData
{
	//boost::shared_ptr<SimpleQuote>  d1wRate;
	//boost::shared_ptr<SimpleQuote>  d1mRate;
	boost::shared_ptr<SimpleQuote>  d3mRate;
	//boost::shared_ptr<SimpleQuote>  d6mRate;
	//boost::shared_ptr<SimpleQuote>  d9mRate;
	//boost::shared_ptr<SimpleQuote>  d1yRate;
	
	boost::shared_ptr<SimpleQuote>  fra3x6Rate;
	boost::shared_ptr<SimpleQuote>  fra6x9Rate;
	boost::shared_ptr<SimpleQuote>  fra9x12Rate;

	boost::shared_ptr<SimpleQuote>  s2yRate;
	boost::shared_ptr<SimpleQuote>  s3yRate;
	boost::shared_ptr<SimpleQuote>  s4yRate;
	boost::shared_ptr<SimpleQuote>  s5yRate;
	boost::shared_ptr<SimpleQuote>  s6yRate;
	boost::shared_ptr<SimpleQuote>  s7yRate;
	boost::shared_ptr<SimpleQuote>  s8yRate;
	boost::shared_ptr<SimpleQuote>  s9yRate;
	boost::shared_ptr<SimpleQuote>  s10yRate;
	boost::shared_ptr<SimpleQuote>  s12yRate;
	boost::shared_ptr<SimpleQuote>  s15yRate;

	CurveData() :
		//d1wRate(new SimpleQuote()),
		//d1mRate(new SimpleQuote()),
		d3mRate(new SimpleQuote()),
		//d6mRate(new SimpleQuote()),
		//d9mRate(new SimpleQuote()),
		//d1yRate(new SimpleQuote()),

		fra3x6Rate(new SimpleQuote()),
		fra6x9Rate(new SimpleQuote()),
		fra9x12Rate(new SimpleQuote()),

		s2yRate(new SimpleQuote()),
		s3yRate(new SimpleQuote()),
		s4yRate(new SimpleQuote()),
		s5yRate(new SimpleQuote()),
		s6yRate(new SimpleQuote()),
		s7yRate(new SimpleQuote()),
		s8yRate(new SimpleQuote()),
		s9yRate(new SimpleQuote()),
		s10yRate(new SimpleQuote()),
		s12yRate(new SimpleQuote()),
		s15yRate(new SimpleQuote())
	{
	}
};

struct OISCurveData
{
	boost::shared_ptr<SimpleQuote>  o1wRate;
	boost::shared_ptr<SimpleQuote>  o2wRate;
	boost::shared_ptr<SimpleQuote>  o3wRate;
	boost::shared_ptr<SimpleQuote>  o1mRate;
	boost::shared_ptr<SimpleQuote>  o2mRate;
	boost::shared_ptr<SimpleQuote>  o3mRate;
	boost::shared_ptr<SimpleQuote>  o6mRate;
	boost::shared_ptr<SimpleQuote>  o9mRate;
	boost::shared_ptr<SimpleQuote>  o1yRate;
	boost::shared_ptr<SimpleQuote>  o2yRate;
	boost::shared_ptr<SimpleQuote>  o15yRate;
	
	OISCurveData() :
		o1wRate(new SimpleQuote()),
		o2wRate(new SimpleQuote()),
		o3wRate(new SimpleQuote()),
		o1mRate(new SimpleQuote()),
		o2mRate(new SimpleQuote()),
		o3mRate(new SimpleQuote()),
		o6mRate(new SimpleQuote()),
		o9mRate(new SimpleQuote()),
		o1yRate(new SimpleQuote()),
		o2yRate(new SimpleQuote()),
		o15yRate(new SimpleQuote())

	{
	}
};

// Fill out the curve with sample market data
void sampleMktDataFile(CurveData &cd)
{
	string line;
	ifstream myfile("depoFRAswap.txt", ios::in);
	if (myfile.is_open())
	{
		//getline(myfile, line); *cd.d1wRate = atof(line.substr(line.find(" ") + 1).c_str());
		//getline(myfile, line); *cd.d1mRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.d3mRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.fra3x6Rate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.fra6x9Rate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.fra9x12Rate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.s2yRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.s3yRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.s4yRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.s5yRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.s6yRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.s7yRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.s8yRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.s9yRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.s10yRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.s12yRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.s15yRate = atof(line.substr(line.find(" ") + 1).c_str());
		myfile.close();
	}
	else cout << "Unable to open file";
}

void OISsampleMktDataFile(OISCurveData &cd)
{
	string line;
	ifstream myfile("OIS.txt", ios::in);
	if (myfile.is_open())
	{
		getline(myfile, line); *cd.o1wRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.o2wRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.o3wRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.o1mRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.o2mRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.o3mRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.o6mRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.o9mRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.o1yRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.o2yRate = atof(line.substr(line.find(" ") + 1).c_str());
		getline(myfile, line); *cd.o15yRate = atof(line.substr(line.find(" ") + 1).c_str());
		myfile.close();
	}
	else cout << "Unable to open file";
}

void sampleMktData(CurveData &cd)
{	
	*cd.d3mRate = 0.0007800;
	*cd.fra3x6Rate = 0.0007000;
	*cd.fra6x9Rate = 0.0006000;
	*cd.fra9x12Rate = 0.0008050;
	*cd.s2yRate = 0.00177;
	*cd.s3yRate = 0.00224;
	*cd.s4yRate = 0.00287;
	*cd.s5yRate = 0.0036;
	*cd.s6yRate = 0.00442;
	*cd.s7yRate = 0.00532;
	*cd.s8yRate = 0.00629;
	*cd.s9yRate = 0.00727;
	*cd.s10yRate = 0.008195;
	*cd.s12yRate = 0.009788;
	*cd.s15yRate = 0.011528;
}

void OISsampleMktData(OISCurveData &cd)
{
	*cd.o1wRate = -0.00067;
	*cd.o2wRate = -0.00067;
	*cd.o3wRate = -0.00063;
	*cd.o1mRate = -0.00044;
	*cd.o2mRate = -0.00021;
	*cd.o3mRate = -0.00026;
	*cd.o6mRate = -0.00049;
	*cd.o9mRate = -0.00065;
	*cd.o1yRate = -0.00070;
	*cd.o2yRate = -0.00077;
	*cd.o15yRate = 0.008590;
}

void OISbump(OISCurveData &OIScd, Real BpsShift)
{
	*OIScd.o1wRate = OIScd.o1wRate->value() + BpsShift;
	*OIScd.o2wRate = OIScd.o2wRate->value() + BpsShift;
	*OIScd.o3wRate = OIScd.o3wRate->value() + BpsShift;
	*OIScd.o1mRate = OIScd.o1mRate->value() + BpsShift;
	*OIScd.o2mRate = OIScd.o2mRate->value() + BpsShift;
	*OIScd.o3mRate = OIScd.o3mRate->value() + BpsShift;
	*OIScd.o6mRate = OIScd.o6mRate->value() + BpsShift;
	*OIScd.o9mRate = OIScd.o9mRate->value() + BpsShift;
	*OIScd.o1yRate = OIScd.o1yRate->value() + BpsShift;
	*OIScd.o2yRate = OIScd.o2yRate->value() + BpsShift;
	*OIScd.o15yRate = OIScd.o15yRate->value() + BpsShift;
}

// Build yield curves. Note that the resulting curves are linked to the supplied data and will change with it
boost::shared_ptr<YieldTermStructure> buildCurveOIS(const OISCurveData &cd, Date todaysDate, Calendar calendar)
{
	Integer fixingDays = 2;
	Settings::instance().evaluationDate() = todaysDate;

	Date settlementDate = calendar.advance(todaysDate, fixingDays, Days);
	// must be a business day
	settlementDate = calendar.adjust(settlementDate);

	// setup OIS
	Natural settlementDays = 2;
	boost::shared_ptr<OvernightIndex> overnightIndex(new Eonia);

	//boost::shared_ptr<Quote> donRate(new SimpleQuote(0.0001));
	//DayCounter depositDayCounter = Actual360();
	//boost::shared_ptr<RateHelper> don(new DepositRateHelper(Handle<Quote>(donRate),	1 * Days, fixingDays, calendar, ModifiedFollowing,	true, depositDayCounter));
	boost::shared_ptr<RateHelper> o1w(new OISRateHelper(settlementDays, 1 * Weeks, Handle<Quote>(cd.o1wRate), overnightIndex));
	boost::shared_ptr<RateHelper> o2w(new OISRateHelper(settlementDays, 2 * Weeks, Handle<Quote>(cd.o2wRate), overnightIndex));
	boost::shared_ptr<RateHelper> o3w(new OISRateHelper(settlementDays, 3 * Weeks, Handle<Quote>(cd.o3wRate), overnightIndex));
	boost::shared_ptr<RateHelper> o1m(new OISRateHelper(settlementDays, 1 * Months, Handle<Quote>(cd.o1mRate), overnightIndex));
	boost::shared_ptr<RateHelper> o2m(new OISRateHelper(settlementDays, 2 * Months, Handle<Quote>(cd.o2mRate), overnightIndex));
	boost::shared_ptr<RateHelper> o3m(new OISRateHelper(settlementDays, 3 * Months, Handle<Quote>(cd.o3mRate), overnightIndex));
	boost::shared_ptr<RateHelper> o6m(new OISRateHelper(settlementDays, 6 * Months, Handle<Quote>(cd.o6mRate), overnightIndex));
	boost::shared_ptr<RateHelper> o9m(new OISRateHelper(settlementDays, 9 * Months, Handle<Quote>(cd.o9mRate), overnightIndex));
	boost::shared_ptr<RateHelper> o1y(new OISRateHelper(settlementDays, 1 * Years, Handle<Quote>(cd.o1yRate), overnightIndex));
	boost::shared_ptr<RateHelper> o2y(new OISRateHelper(settlementDays, 2 * Years, Handle<Quote>(cd.o2yRate), overnightIndex));
	boost::shared_ptr<RateHelper> o15y(new OISRateHelper(settlementDays, 15 * Years, Handle<Quote>(cd.o15yRate), overnightIndex));
	/*
	Frequency swFixedLegFrequency = Annual;
	BusinessDayConvention swFixedLegConvention = Unadjusted;
	DayCounter swFixedLegDayCounter = Thirty360(Thirty360::European);
	boost::shared_ptr<IborIndex> swFloatingLegIndex(new Eonia);
	boost::shared_ptr<RateHelper> o1w(new SwapRateHelper(Handle<Quote>(cd.o1wRate), 1 * Weeks, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));
	boost::shared_ptr<RateHelper> o2w(new SwapRateHelper(Handle<Quote>(cd.o2wRate), 2 * Weeks, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));
	boost::shared_ptr<RateHelper> o3w(new SwapRateHelper(Handle<Quote>(cd.o3wRate), 3 * Weeks, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));
	boost::shared_ptr<RateHelper> o1m(new SwapRateHelper(Handle<Quote>(cd.o1mRate), 1 * Months, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));
	boost::shared_ptr<RateHelper> o2m(new SwapRateHelper(Handle<Quote>(cd.o2mRate), 2 * Months, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));
	boost::shared_ptr<RateHelper> o3m(new SwapRateHelper(Handle<Quote>(cd.o3mRate), 3 * Months, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));
	boost::shared_ptr<RateHelper> o6m(new SwapRateHelper(Handle<Quote>(cd.o6mRate), 6 * Months, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));
	boost::shared_ptr<RateHelper> o9m(new SwapRateHelper(Handle<Quote>(cd.o9mRate), 9 * Months, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));
	boost::shared_ptr<RateHelper> o1y(new SwapRateHelper(Handle<Quote>(cd.o1yRate), 1 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));
	boost::shared_ptr<RateHelper> o2y(new SwapRateHelper(Handle<Quote>(cd.o2yRate), 2 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));
	*/
	//*********************
	//**  CURVE BUILDING **
	//*********************

	// Any DayCounter would be fine.
	// ActualActual::ISDA ensures that 30 years is 30.0
	DayCounter termStructureDayCounter = ActualActual(ActualActual::ISDA);
	double tolerance = 1.0e-15;

	// A depo-FRA-swap curve
	std::vector<boost::shared_ptr<RateHelper> > OISInstruments;
	//OISInstruments.push_back(don);
	OISInstruments.push_back(o1w);
	OISInstruments.push_back(o2w);
	OISInstruments.push_back(o3w);
	OISInstruments.push_back(o1m);
	OISInstruments.push_back(o2m);
	OISInstruments.push_back(o3m);
	OISInstruments.push_back(o6m);
	OISInstruments.push_back(o9m);
	OISInstruments.push_back(o1y);
	OISInstruments.push_back(o2y);
	OISInstruments.push_back(o15y);
	boost::shared_ptr<YieldTermStructure> OISTermStructure(
		new PiecewiseYieldCurve<Discount, LogLinear>(
		settlementDate, OISInstruments,
		termStructureDayCounter,
		tolerance));

	return OISTermStructure;
}

boost::shared_ptr<YieldTermStructure> buildCurve(const CurveData &cd, Date todaysDate, Calendar calendar, const Handle<YieldTermStructure>& discountingCurve = Handle<YieldTermStructure>())
{
	Integer fixingDays = 2;
	Settings::instance().evaluationDate() = todaysDate;

	Date settlementDate = calendar.advance(todaysDate, fixingDays, Days);
	// must be a business day
	settlementDate = calendar.adjust(settlementDate);

	// setup deposits
	DayCounter depositDayCounter = Actual360();

	//boost::shared_ptr<RateHelper> d1w(new DepositRateHelper(Handle<Quote>(cd.d1wRate),	1 * Weeks, fixingDays, calendar, ModifiedFollowing,	true, depositDayCounter));
	//boost::shared_ptr<RateHelper> d1m(new DepositRateHelper(Handle<Quote>(cd.d1mRate),	1 * Months, fixingDays,	calendar, ModifiedFollowing, true, depositDayCounter));
	boost::shared_ptr<RateHelper> d3m(new DepositRateHelper(Handle<Quote>(cd.d3mRate), 3 * Months, fixingDays, calendar, ModifiedFollowing, true, depositDayCounter));

	// setup FRAs
	boost::shared_ptr<RateHelper> fra3x6(new FraRateHelper(Handle<Quote>(cd.fra3x6Rate), 3, 6, fixingDays, calendar, ModifiedFollowing, true, depositDayCounter));
	boost::shared_ptr<RateHelper> fra6x9(new FraRateHelper(Handle<Quote>(cd.fra6x9Rate), 6, 9, fixingDays, calendar, ModifiedFollowing, true, depositDayCounter));
	boost::shared_ptr<RateHelper> fra9x12(new FraRateHelper(Handle<Quote>(cd.fra9x12Rate), 9, 12, fixingDays, calendar, ModifiedFollowing, true, depositDayCounter));

	// setup swaps
	//USD convention: Semi 30/360 fixed vs qtrly USD LIBOR3M ACT/360 floating
	//EUR convention: 1y is ann 30/360 fixed vs qtrly EURIBOR3M ACT/360 floating
	//EUR: Everything else is ann 30 / 360 fixed vs semi EURIBOR6M ACT / 360 floating
	Frequency swFixedLegFrequency = Annual;
	BusinessDayConvention swFixedLegConvention = Unadjusted;
	DayCounter swFixedLegDayCounter = Thirty360(Thirty360::European);
	//boost::shared_ptr<IborIndex> swFloatingLegIndex(new Euribor6M(discountingCurve));
	boost::shared_ptr<IborIndex> swFloatingLegIndex(new Euribor6M);
	//boost::shared_ptr<IborIndex> swFloatingLegIndex(new Libor);

	boost::shared_ptr<RateHelper> s2y(new SwapRateHelper(Handle<Quote>(cd.s2yRate), 2 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex, Handle<Quote>(), 0 * Days, discountingCurve));
	boost::shared_ptr<RateHelper> s3y(new SwapRateHelper(Handle<Quote>(cd.s3yRate), 3 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex, Handle<Quote>(), 0 * Days, discountingCurve));
	boost::shared_ptr<RateHelper> s4y(new SwapRateHelper(Handle<Quote>(cd.s4yRate), 4 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex, Handle<Quote>(), 0 * Days, discountingCurve));
	boost::shared_ptr<RateHelper> s5y(new SwapRateHelper(Handle<Quote>(cd.s5yRate), 5 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex, Handle<Quote>(), 0 * Days, discountingCurve));
	boost::shared_ptr<RateHelper> s6y(new SwapRateHelper(Handle<Quote>(cd.s6yRate), 6 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex, Handle<Quote>(), 0 * Days, discountingCurve));
	boost::shared_ptr<RateHelper> s7y(new SwapRateHelper(Handle<Quote>(cd.s7yRate), 7 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex, Handle<Quote>(), 0 * Days, discountingCurve));
	boost::shared_ptr<RateHelper> s8y(new SwapRateHelper(Handle<Quote>(cd.s8yRate), 8 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex, Handle<Quote>(), 0 * Days, discountingCurve));
	boost::shared_ptr<RateHelper> s9y(new SwapRateHelper(Handle<Quote>(cd.s9yRate), 9 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex, Handle<Quote>(), 0 * Days, discountingCurve));
	boost::shared_ptr<RateHelper> s10y(new SwapRateHelper(Handle<Quote>(cd.s10yRate), 10 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex, Handle<Quote>(), 0 * Days, discountingCurve));
	boost::shared_ptr<RateHelper> s12y(new SwapRateHelper(Handle<Quote>(cd.s12yRate), 12 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex, Handle<Quote>(), 0 * Days, discountingCurve));
	boost::shared_ptr<RateHelper> s15y(new SwapRateHelper(Handle<Quote>(cd.s15yRate), 15 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex, Handle<Quote>(), 0 * Days, discountingCurve));
	
	//*********************
	//**  CURVE BUILDING **
	//*********************

	// Any DayCounter would be fine.
	// ActualActual::ISDA ensures that 30 years is 30.0
	DayCounter termStructureDayCounter = ActualActual(ActualActual::ISDA);
	double tolerance = 1.0e-15;

	// A depo-FRA-swap curve
	std::vector<boost::shared_ptr<RateHelper> > depoFRASwapInstruments;
	//depoFRASwapInstruments.push_back(d1w);
	//depoFRASwapInstruments.push_back(d1m);
	depoFRASwapInstruments.push_back(d3m);
	depoFRASwapInstruments.push_back(fra3x6);
	depoFRASwapInstruments.push_back(fra6x9);
	depoFRASwapInstruments.push_back(fra9x12);
	depoFRASwapInstruments.push_back(s2y);
	depoFRASwapInstruments.push_back(s3y);
	depoFRASwapInstruments.push_back(s4y);
	depoFRASwapInstruments.push_back(s5y);
	depoFRASwapInstruments.push_back(s6y);
	depoFRASwapInstruments.push_back(s7y);
	depoFRASwapInstruments.push_back(s8y);
	depoFRASwapInstruments.push_back(s9y);
	depoFRASwapInstruments.push_back(s10y);
	depoFRASwapInstruments.push_back(s12y);
	depoFRASwapInstruments.push_back(s15y);
	boost::shared_ptr<YieldTermStructure> depoFRASwapTermStructure(
		new PiecewiseYieldCurve<Discount, LogLinear>(
		settlementDate, depoFRASwapInstruments,
		termStructureDayCounter,
		tolerance));

	return depoFRASwapTermStructure;
}


void dual_curve_test()
{
	CurveData cd;
	OISCurveData OIScd;

	// Fill out with some sample market data
	sampleMktData(cd);
	OISsampleMktData(OIScd);

	Date todaysDate = Date(31, Dec, 2014);
	Calendar calendar = TARGET();

	// Term structures that will be used for pricing:
	// the one used for discounting cash flows
	RelinkableHandle<YieldTermStructure> discountingTermStructure;
	// the one used for forward rate forecasting	
	RelinkableHandle<YieldTermStructure> forecastingTermStructure;
	
	// Build a curve linked to this market data
	boost::shared_ptr<YieldTermStructure> OISTermStructure = buildCurveOIS(OIScd, todaysDate, calendar);
	discountingTermStructure.linkTo(OISTermStructure);
	//Handle<YieldTermStructure> discountingTermStructure(OISTermStructure);
	
	//Now make forecastingTermStructure in dual-curve mode:
	boost::shared_ptr<YieldTermStructure> depoFRASwapTermStructure = buildCurve(cd, todaysDate, calendar, discountingTermStructure);
	//boost::shared_ptr<YieldTermStructure> depoFRASwapTermStructure = buildCurve(cd, todaysDate, calendar, OISTermStructure);
	//Alternatively make forecastingTermStructure in single-curve mode:
	//boost::shared_ptr<YieldTermStructure> depoFRASwapTermStructure = buildCurve(cd, todaysDate, calendar);
	forecastingTermStructure.linkTo(depoFRASwapTermStructure);
	
	ofstream fileout1("discCurve.txt", ios::out);
	ofstream fileout2("forcCurve.txt", ios::out);
	for (int i = 2; i < 730; i++) {
		Date tempDate = calendar.advance(todaysDate, i, Days);
		//fileout1 << tempDate << ": " << discountingTermStructure->zeroRate(tempDate, Actual360(), Continuous).rate() << endl;
		fileout1 << tempDate << ": " << discountingTermStructure->discount(tempDate, true) << endl;
		fileout2 << tempDate << ": " << forecastingTermStructure->discount(tempDate, true) << endl;
	}
	fileout1.close();
	fileout2.close();
	
	// Print original curve
	std::cout << "Zero rates from original LIBOR curve:" << std::endl;
	for (size_t i = 1; i<=16; ++i)
		std::cout << io::short_date(calendar.advance(todaysDate, 3 * i, Months)) << boost::format(": %g") % forecastingTermStructure->zeroRate(calendar.advance(todaysDate, 3 * i, Months), Actual360(), Compounded).rate() << std::endl;
	std::cout << std::endl << std::endl;

	// Parallel shift of the OIS curve
	OISbump(OIScd,0.01);

	// Print the bumped, rebuilt curve
	std::cout << "Zero rates from LIBOR curve after OIS going up by 0.01:" << std::endl;
	for (size_t i = 1; i<=16; ++i)
		std::cout << io::short_date(calendar.advance(todaysDate, 3 * i, Months)) << boost::format(": %g") % forecastingTermStructure->zeroRate(calendar.advance(todaysDate, 3 * i, Months), Actual360(), Compounded).rate() << std::endl;
	std::cout << std::endl;
}

int main(int, char*[]) {
	try {
		dual_curve_test();
		system("pause");
		return 0;
	}
	catch (exception& e) {
		cerr << e.what() << endl;
		return 1;
	}
	catch (...) {
		cerr << "unknown error" << endl;
		return 1;
	}
}
