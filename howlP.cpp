#include <iostream>
#include <vector>
#include <algorithm> 
#include <time.h>

template <class messageType>
void pOL(messageType message){
    std::cout << message;
}

class storage{

    // There is no need to share this containers and methods with derrived class
    // and main program
    private:
    // messages represent as strings
	std::string modeHeader = "\nChoose your goal of research: ";
    std::string incorrect = "\nThis input is incorrect.\n";
    std::string comeAgain = "\nThat's all for now, run again.\n";
	
	//
	public:
	//
	char answer;
	//
	bool mode = true;
	//
	bool approved = true;

	//
	void openDialog(){
		modeHeader += "\n\n(s/S) - for varifing single number";
		modeHeader += " being Howling prime,";
		modeHeader += "\n(r/R) - for showing all Howling prime numbers";
		modeHeader += " in the given range.\n\n";
		pOL(modeHeader);
		std::cin >> answer;
	}

	//
	void validateDialog(){
		// Setting range-search mode
		if ( (answer == 's') || (answer == 'S') )
			mode = true;
		// Setting range-search mode
		else if ( (answer == 'r') || (answer == 'R') )
			mode = false;
		// Not allow to program work when input invalid
		else approved = false;
		//
		if (!approved) pOL(incorrect);
	}

	//
	virtual void getNumber(){}
	//
	virtual void getRange(){}
	//
	virtual void operate(){}
	//
	virtual void closeDialog(){}
	//
	virtual ~storage(){
		pOL(comeAgain);
	}
};

class numberRes : public storage {
	//
	private:
	// messages represent as strings for range research dialogs
    std::string enterNumber = "\nYou entered number for research: ";
    std::string HowlingPrime = "\nYou enter a Howling prime number.\n";
    std::string notPrime = "\nIt's not a prime number.\n";
    std::string justPrime = "\nIt's just a prime number, but not Howling((\n";   

	//
	unsigned long long int number;
    
    //
	protected:
    //
    clock_t prime_start;
    //
    clock_t prime_pause;
    //
    clock_t time_new;
    //
    clock_t start_operate;
    //
    unsigned long long int new_point;
    //
    std::vector<unsigned long long int> pVec;
    
	// bool container, true - prime number, false - not prime
	bool prime_status = false;
	// bool container, true - prime number, false - not prime
	bool sum_status = false;
	// bool container, true - time error, false - everything fine
	bool time_failure = false;

	// Method calculate all primes for end point
	void getPrimes(unsigned long long int &point){

		// temporary container
		unsigned long long int k;
		// temporary container
		unsigned long long int y = point/11;
		// first number in the container
		pVec.push_back(2);
		pVec.push_back(3);
		pVec.push_back(5);
		pVec.push_back(7);
		pVec.push_back(11);

	    time_failure = false;
	    prime_start = clock();

		// calculate prime numbers with odd divisors
		for (unsigned long long int i = 11; ( ( i <= y ) && !time_failure ); ){

	        prime_pause = clock() - prime_start;
			if ( prime_pause*1.0/CLOCKS_PER_SEC > 1200 ){
				point = i*11;
				new_point = point;
				time_failure = true;		
			}

			bool result  = true;

			k = i/2;

			// first divise number with prime that alreay in the list
			for (std::vector<unsigned long long int>::iterator it = pVec.begin();
			     ( ( it != pVec.end() ) && ( *it <= k ) && result); it++){
			     		// if something left from division this not prime
			     		if ( !( i % (*it) ) ) result = false;
			}

			k++;
			
			// then find divisors from other odd numbers
			for (unsigned long long int j = pVec.back(); ( ( j <= k ) && result ); ){
				// if something left from division this not prime
				if ( !( i % j ) ) result = false;
				// to the next odd number
				j += 2;
			}
			// puch to vector only primes
			if ( result ) pVec.push_back(i);

			i += 2;
		}

		prime_pause = clock() - prime_start;
		pOL("\nTime spend on calculation primes: ");
		pOL(prime_pause*1.0/CLOCKS_PER_SEC);
		pOL(" sec.\n");

	}

	// Method check if number is prime
	bool checkPrime(unsigned long long int x){
	    //
		for (std::vector<unsigned long long int>::iterator it = pVec.begin();
		     it != pVec.end(); it++){
			//
			if (x == (*it)) return true;
			if ( ( x > (*it) ) && !(x % (*it)) ) return false;
		}
	    return true;
	}

	// Method return true in case number being prime
	unsigned long long int getSum(unsigned long long int x){
		//
	    unsigned long long int c = 0;

	    while ( x > 0 ){
	        c += (x%10);
	        x /= 10;
	    }
	    return c;
	}

	//
	public:
	//
	void getNumber(){
		pOL(enterNumber);
		std::cin >> number;
	}
	//
	virtual void getRange(){}
	//
	virtual void operate(){}
	//
	void closeDialog(){
		getPrimes(number);
		unsigned long long int k = getSum(number);
		if (checkPrime(number)){
			if (checkPrime(k)) pOL(HowlingPrime);
			else pOL(justPrime);
		}
		else pOL(notPrime);
	}
};

//
class rangeRes : public numberRes {
	//
	private:
	// messages represent as strings for range research dialogs
    std::string enterLrange = "\nEnter first number in the range: ";
    std::string enterRrange = "\nEnter last number in the range: ";
    std::string HowlingRange = "\nAll Howling primes in the given range [";
    std::string limitRange = "\nAll Howling primes in the corrected range [";
    std::string back = "] are:\n\n{ ";
    std::string notMe = "\nNot a single Howling prime in the given range((\n";
    std::string limit = "\nCalculations ended with time limit error.\n";
    //
    clock_t prime_start;
    //
    clock_t prime_pause;
    //
    clock_t time_new;
    //
    clock_t start_operate;    

    //
    std::vector<unsigned long long int> howSVec;
    //
    std::vector<unsigned long long int> howDVec;
    //
    std::vector<unsigned long long int> howTVec;
	// bool container, true - prime number, false - not prime
	bool prime_status = false;
	// bool container, true - prime number, false - not prime
	bool sum_status = false;
	// bool container, true - time error, false - everything fine
	bool time_failure = false;

	//
	unsigned long long int leftRange;
    //
    unsigned long long int rightRange;
	//
	unsigned long long int LIMITrRange;	

	//
	public:

	//
	void getRange(){
		pOL(enterLrange);
		std::cin >> leftRange;
		pOL(enterRrange);
		std::cin >> rightRange;
	}

	//
	void operate(){
	    //unsigned long long int newn;
	    //
	    time_failure = false;

	    unsigned long long int k;
	    unsigned long long int j;
	    unsigned long long int z;

	    getPrimes(rightRange);
		// keep old end of range if not
	    LIMITrRange = rightRange;

	    start_operate = clock();

	    for (unsigned long long int next = leftRange; next <= rightRange; ){        

	        k = getSum(next);
	        if ( checkPrime(next) && checkPrime(k) ) howSVec.push_back(next);	 

			// pause clock and count time of loop break if limit resieved
	        time_new = clock() - start_operate;
			if ( time_new*1.0/CLOCKS_PER_SEC > 1200 ) time_failure = true;

			// keep new end of range in case time exeception
	        if ( time_failure ){
	        	LIMITrRange = next-1;
	        	break;
	        }

	        if (next < 3) next++;
	        else next +=2;
	    }        
	}

	//
	void closeDialog(){
		//
		int i = 0;
		
		if (howSVec.empty()) pOL(notMe);
		
		else {

			// output all Howling prime numbes
		    for (std::vector<unsigned long long int>::iterator it = howSVec.begin();
		        it != howSVec.end(); it++){

		        pOL(*it);
		    	pOL('\t');
		        if (++i == 6){
		            pOL('\n');
		            i = 0;
		        }
		    }

			if (time_failure){
				pOL(limit);
				pOL(limitRange);
			}
			else pOL(HowlingRange);
			
			if ( prime_pause*1.0/CLOCKS_PER_SEC > 1200 ){
				pOL("\nCalculation of prime number divisors ended with time limit.\n");
				pOL("\nMaded first correction of last number in the range to -> ");
				pOL(new_point);
				pOL(".\n");
			}

		    if ( time_new*1.0/CLOCKS_PER_SEC > 1200 ){
				pOL("\nTime failure in operate() on number next = ");
				pOL(LIMITrRange+1);
				pOL(".\n");
			}	

	    	pOL(leftRange);
	    	pOL(", ");
		    pOL(LIMITrRange);
		    pOL("]:\n\n");
		    pOL("\n{\n");

		    pOL("\n}.\n\nQuantity of single-time howling primes: ");
		    pOL(howSVec.size());
		    pOL(".\n");
		}
	}
};

int main()
{
	clock_t t;
	
    storage *entry = new storage;

    entry -> openDialog();

    entry -> validateDialog();

    if ( (entry -> approved) && (entry -> mode) ) {

    	storage *check = new numberRes;

	    check -> getNumber();
		
		t = clock();

	    check -> closeDialog();
	}
    if ( (entry -> approved) && !(entry -> mode) ) {

	    storage *check = new rangeRes;

	    check -> getRange();
		
		t = clock();
	    
	    check -> operate();

	    check -> closeDialog();
	}

	delete entry;

	t = clock() - t;
	pOL("\nTime program spend working: ");
	pOL(t*1.0/CLOCKS_PER_SEC);
    pOL(" sec.\n");

    return 0;
}
