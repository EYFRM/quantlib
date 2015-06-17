#include <ql/quantlib.hpp>
#include <boost/timer.hpp>
#include <iostream>
#include <iomanip>

using namespace QuantLib;

#define LENGTH(a) (sizeof(a)/sizeof(a[0]))

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {
	Integer sessionId() { return 0; }
}
#endif

void printCalibration(const std::vector<boost::shared_ptr<CalibrationHelper> >& helpers, Integer *swapLenghts, Volatility *swaptionVols, Size swapNumber) {
	// Output the implied Black volatilities
	Size numRows = swapNumber;
	Size numCols = swapNumber;
	for (Size i = 0; i<numRows; i++) {
		Size j = numCols - i - 1; // 1x5, 2x4, 3x3, 4x2, 5x1
		Size k = i*numCols + j;
		Real npv = helpers[i]->modelValue();
		Volatility implied = helpers[i]->impliedVolatility(npv, 1e-4,
			1000, 0.05, 0.50);
		Volatility diff = implied - swaptionVols[k];

		std::cout << i + 1 << "x" << swapLenghts[j]
			<< std::setprecision(5) << std::noshowpos
			<< ": model " << std::setw(7) << io::volatility(implied)
			<< ", market " << std::setw(7)
			<< io::volatility(swaptionVols[k])
			<< " (" << std::setw(7) << std::showpos
			<< io::volatility(diff) << std::noshowpos << ")\n";
	}
}

void printParameters(const boost::shared_ptr<ShortRateModel>& model) {
	std::string Field[] = { "a", "sigma", "b", "eta", "rho" };
	Size numParams = model->params().size();
	std::cout << "calibrated to:\n";
	for (Size i = 0; i<numParams-1; i++) {
		std::cout << Field[i] << " = " << model->params()[i] << ", ";
	}
	std::cout << Field[numParams - 1] << " = " << model->params()[numParams - 1];
	std::cout << std::endl << std::endl;
}

void hw_test(Integer *swapLenghts, Volatility *swaptionVols, Size swapNumber){
	boost::timer timer;
	std::cout << std::endl;

	Date todaysDate(15, February, 2002);
	Calendar calendar = TARGET();
	Date settlementDate(19, February, 2002);
	Settings::instance().evaluationDate() = todaysDate;

	// flat yield term structure impling 1x5 swap at 5%
	boost::shared_ptr<Quote> flatRate(new SimpleQuote(0.04875825));
	Handle<YieldTermStructure> rhTermStructure(
		boost::shared_ptr<FlatForward>(new FlatForward(settlementDate, Handle<Quote>(flatRate),Actual365Fixed())));

	// Define the ATM/OTM/ITM swaps
	Frequency fixedLegFrequency = Annual;
	BusinessDayConvention fixedLegConvention = Unadjusted;
	BusinessDayConvention floatingLegConvention = ModifiedFollowing;
	DayCounter fixedLegDayCounter = Thirty360(Thirty360::European);
	Frequency floatingLegFrequency = Semiannual;
	VanillaSwap::Type type = VanillaSwap::Payer;
	Rate dummyFixedRate = 0.03;
	boost::shared_ptr<IborIndex> indexSixMonths(new
		Euribor6M(rhTermStructure));

	Date startDate = calendar.advance(settlementDate, 1, Years,
		floatingLegConvention);
	Date maturity = calendar.advance(startDate, 5, Years,
		floatingLegConvention);
	Schedule fixedSchedule(startDate, maturity, Period(fixedLegFrequency),
		calendar, fixedLegConvention, fixedLegConvention,
		DateGeneration::Forward, false);
	Schedule floatSchedule(startDate, maturity, Period(floatingLegFrequency),
		calendar, floatingLegConvention, floatingLegConvention,
		DateGeneration::Forward, false);

	boost::shared_ptr<VanillaSwap> swap(new VanillaSwap(
		type, 1000.0,
		fixedSchedule, dummyFixedRate, fixedLegDayCounter,
		floatSchedule, indexSixMonths, 0.0,
		indexSixMonths->dayCounter()));
	swap->setPricingEngine(boost::shared_ptr<PricingEngine>(
		new DiscountingSwapEngine(rhTermStructure)));
	Rate fixedATMRate = swap->fairRate();
	Rate fixedOTMRate = fixedATMRate * 1.2;
	Rate fixedITMRate = fixedATMRate * 0.8;

	boost::shared_ptr<VanillaSwap> atmSwap(new VanillaSwap(
		type, 1000.0,
		fixedSchedule, fixedATMRate, fixedLegDayCounter,
		floatSchedule, indexSixMonths, 0.0,
		indexSixMonths->dayCounter()));
	boost::shared_ptr<VanillaSwap> otmSwap(new VanillaSwap(
		type, 1000.0,
		fixedSchedule, fixedOTMRate, fixedLegDayCounter,
		floatSchedule, indexSixMonths, 0.0,
		indexSixMonths->dayCounter()));
	boost::shared_ptr<VanillaSwap> itmSwap(new VanillaSwap(
		type, 1000.0,
		fixedSchedule, fixedITMRate, fixedLegDayCounter,
		floatSchedule, indexSixMonths, 0.0,
		indexSixMonths->dayCounter()));

	// defining the swaptions to be used in model calibration
	std::vector<Period> swaptionMaturities;
	swaptionMaturities.push_back(Period(1, Years));
	swaptionMaturities.push_back(Period(2, Years));
	swaptionMaturities.push_back(Period(3, Years));
	swaptionMaturities.push_back(Period(4, Years));
	swaptionMaturities.push_back(Period(5, Years));

	std::vector<boost::shared_ptr<CalibrationHelper> > swaptions;

	// List of times that have to be included in the timegrid
	std::list<Time> times;

	Size numRows = swapNumber;
	Size numCols = swapNumber;
	Size i;
	for (i = 0; i<numRows; i++) {
		Size j = numCols - i - 1; // 1x5, 2x4, 3x3, 4x2, 5x1
		Size k = i*numCols + j;
		boost::shared_ptr<Quote> vol(new SimpleQuote(swaptionVols[k]));
		swaptions.push_back(boost::shared_ptr<CalibrationHelper>(new
			SwaptionHelper(swaptionMaturities[i],
			Period(swapLenghts[j], Years),
			Handle<Quote>(vol),
			indexSixMonths,
			indexSixMonths->tenor(),
			indexSixMonths->dayCounter(),
			indexSixMonths->dayCounter(),
			rhTermStructure)));
		swaptions.back()->addTimesTo(times);
	}

	// Building time-grid
	TimeGrid grid(times.begin(), times.end(), 30);

	// defining the models
	boost::shared_ptr<G2> modelG2(new G2(rhTermStructure));
	boost::shared_ptr<HullWhite> modelHW(new HullWhite(rhTermStructure));
	//boost::shared_ptr<HullWhite> modelHW2(new HullWhite(rhTermStructure));
	//boost::shared_ptr<BlackKarasinski> modelBK(new BlackKarasinski(rhTermStructure));


	// model calibrations
	LevenbergMarquardt om;
	EndCriteria TheEndCriteria = EndCriteria(400, 100, 1.0e-8, 1.0e-8, 1.0e-8);

	std::cout << "Hull-White (analytic formulae) calibration" << std::endl;
	for (i = 0; i<swaptions.size(); i++)
		swaptions[i]->setPricingEngine(boost::shared_ptr<PricingEngine>(new JamshidianSwaptionEngine(modelHW)));

	modelHW->calibrate(swaptions, om, TheEndCriteria);
	printCalibration(swaptions, swapLenghts, swaptionVols,swapNumber);
	printParameters(modelHW);

	std::cout << "G2 (analytic formulae) calibration" << std::endl;
	for (i = 0; i<swaptions.size(); i++)
		swaptions[i]->setPricingEngine(boost::shared_ptr<PricingEngine>(new G2SwaptionEngine(modelG2, 6.0, 16)));

	modelG2->calibrate(swaptions, om, TheEndCriteria);
	printCalibration(swaptions, swapLenghts, swaptionVols, swapNumber);
	printParameters(modelG2);

	//Define dates
	std::vector<Date> bermudanDates;
	const std::vector<boost::shared_ptr<CashFlow> >& leg = swap->fixedLeg();
	for (i = 0; i<leg.size(); i++) {
		boost::shared_ptr<Coupon> coupon = boost::dynamic_pointer_cast<Coupon>(leg[i]);
		bermudanDates.push_back(coupon->accrualStartDate());
	}
	boost::shared_ptr<Exercise> bermudanExercise(new BermudanExercise(bermudanDates));

	// ATM Bermudan swaption pricing
	std::cout << "Payer bermudan swaption struck at " << io::rate(fixedATMRate) << " (ATM)" << std::endl;
	Swaption bermudanSwaption(atmSwap, bermudanExercise);

	// Do the pricing (tree and fdm)
	bermudanSwaption.setPricingEngine(boost::shared_ptr<PricingEngine>(new TreeSwaptionEngine(modelHW, 50)));
	std::cout << "HW (tree):      " << bermudanSwaption.NPV() << std::endl;
	bermudanSwaption.setPricingEngine(boost::shared_ptr<PricingEngine>(new FdHullWhiteSwaptionEngine(modelHW)));
	std::cout << "HW (fdm) :      " << bermudanSwaption.NPV() << std::endl;
	bermudanSwaption.setPricingEngine(boost::shared_ptr<PricingEngine>(new TreeSwaptionEngine(modelG2, 50)));
	std::cout << "G2 (tree):      " << bermudanSwaption.NPV() << std::endl;
	bermudanSwaption.setPricingEngine(boost::shared_ptr<PricingEngine>(new FdG2SwaptionEngine(modelG2)));
	std::cout << "G2 (fdm) :      " << bermudanSwaption.NPV() << std::endl;

	// OTM Bermudan swaption pricing
	std::cout << "Payer bermudan swaption struck at " << io::rate(fixedOTMRate) << " (OTM)" << std::endl;
	Swaption otmBermudanSwaption(otmSwap, bermudanExercise);

	// Do the pricing (tree and fdm)
	otmBermudanSwaption.setPricingEngine(boost::shared_ptr<PricingEngine>(new TreeSwaptionEngine(modelHW, 50)));
	std::cout << "HW (tree):       " << otmBermudanSwaption.NPV() << std::endl;
	otmBermudanSwaption.setPricingEngine(boost::shared_ptr<PricingEngine>(new FdHullWhiteSwaptionEngine(modelHW)));
	std::cout << "HW (fdm) :       " << otmBermudanSwaption.NPV() << std::endl;
	otmBermudanSwaption.setPricingEngine(boost::shared_ptr<PricingEngine>(new TreeSwaptionEngine(modelG2, 50)));
	std::cout << "G2 (tree):      " << otmBermudanSwaption.NPV() << std::endl;
	otmBermudanSwaption.setPricingEngine(boost::shared_ptr<PricingEngine>(new FdG2SwaptionEngine(modelG2)));
	std::cout << "G2 (fdm) :      " << otmBermudanSwaption.NPV() << std::endl;

	// ITM Bermudan swaption pricing
	std::cout << "Payer bermudan swaption struck at " << io::rate(fixedITMRate) << " (ITM)" << std::endl;
	Swaption itmBermudanSwaption(itmSwap, bermudanExercise);

	// Do the pricing (tree and fdm)
	itmBermudanSwaption.setPricingEngine(boost::shared_ptr<PricingEngine>(new TreeSwaptionEngine(modelHW, 50)));
	std::cout << "HW (tree):       " << itmBermudanSwaption.NPV() << std::endl;
	itmBermudanSwaption.setPricingEngine(boost::shared_ptr<PricingEngine>(new FdHullWhiteSwaptionEngine(modelHW)));
	std::cout << "HW (fdm) :       " << itmBermudanSwaption.NPV() << std::endl;
	itmBermudanSwaption.setPricingEngine(boost::shared_ptr<PricingEngine>(new TreeSwaptionEngine(modelG2, 50)));
	std::cout << "G2 (tree):      " << itmBermudanSwaption.NPV() << std::endl;
	itmBermudanSwaption.setPricingEngine(boost::shared_ptr<PricingEngine>(new FdG2SwaptionEngine(modelG2)));
	std::cout << "G2 (fdm) :      " << itmBermudanSwaption.NPV() << std::endl;

	//Make Timer summary
	Real seconds = timer.elapsed();
	Integer hours = int(seconds / 3600);
	seconds -= hours * 3600;
	Integer minutes = int(seconds / 60);
	seconds -= minutes * 60;
	std::cout << " \nRun completed in ";
	if (hours > 0)
		std::cout << hours << " h ";
	if (hours > 0 || minutes > 0)
		std::cout << minutes << " m ";
	std::cout << std::fixed << std::setprecision(0)
		<< seconds << " s\n" << std::endl;
}

int main(int, char*[]) {
	//Number of swaptions to be calibrated to...
	Integer swapLenghts[] = { 1, 2, 3, 4, 5 };
	Volatility swaptionVols[] = {
		0.1490, 0.1340, 0.1228, 0.1189, 0.1148,
		0.1290, 0.1201, 0.1146, 0.1108, 0.1040,
		0.1149, 0.1112, 0.1070, 0.1010, 0.0957,
		0.1047, 0.1021, 0.0980, 0.0951, 0.1270,
		0.1000, 0.0950, 0.0900, 0.1230, 0.1160 };
	Size swapNumber = LENGTH(swapLenghts);
	try {
		hw_test(swapLenghts, swaptionVols, swapNumber);
		system("pause");
		return 0;
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (...) {
		std::cerr << "unknown error" << std::endl;
		return 1;
	}
}
