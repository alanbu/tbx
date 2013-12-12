
#include "cppunit/TestFixture.h"
#include "cppunit/extensions/HelperMacros.h"

#include "tbx/fixed16.h"
#include <sstream>

using namespace tbx;


/**
 * Test fixture to test the Fixed16 class used for a 16 bit fixed
 * point type
 */
class Fixed16Test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( Fixed16Test );

	CPPUNIT_TEST( test_construct );
	CPPUNIT_TEST( test_compare );
	CPPUNIT_TEST( test_output );
	CPPUNIT_TEST( test_input );
	CPPUNIT_TEST( test_sums );
	CPPUNIT_TEST( test_conversions );

	CPPUNIT_TEST_SUITE_END();

public:
	/**
	 * Test fixture setup override
	 */
	void setUp()
	{
	}

	/**
	 * Text fixture tear down override
	 */
	void tearDown()
	{
	}

	/**
	 * Test the component constructors
	 */
	void test_construct()
	{
		Fixed16 one(1);
		Fixed16 minus_one(-1);
		Fixed16 one_pt5(1.5);
		Fixed16 minus_one_pt5(-1.5);
		Fixed16 two(2);
		Fixed16 two_pt5(2.5);

        CPPUNIT_ASSERT(one == 1);
        CPPUNIT_ASSERT(minus_one == -1);
		
        CPPUNIT_ASSERT(one_pt5 == 1.5);
        CPPUNIT_ASSERT(minus_one_pt5 == -1.5);
        CPPUNIT_ASSERT(two == 2);
        CPPUNIT_ASSERT(two_pt5.bits() == 0x28000);
	}


	/**
	 * Check comparison
	 */
	void test_compare()
	{
	   Fixed16 three(3), another_three(three), three_from_double(3.0);
	   Fixed16 one23pt875(123.875);
	   Fixed16 minus_three_pt5(-3.5);

	   CPPUNIT_ASSERT(three == another_three);
	   CPPUNIT_ASSERT(three == three_from_double);
	   CPPUNIT_ASSERT(three == 3);
	   CPPUNIT_ASSERT(3 == three);
	   CPPUNIT_ASSERT(three == 3.0);
	   CPPUNIT_ASSERT(3.0 == three);
	   CPPUNIT_ASSERT(one23pt875 == 123.875);
	   CPPUNIT_ASSERT(123.875 == one23pt875);
	   CPPUNIT_ASSERT(minus_three_pt5 == -3.5);
	   CPPUNIT_ASSERT(-3.5 == minus_three_pt5);

	   CPPUNIT_ASSERT( three < one23pt875 );
	   CPPUNIT_ASSERT( three > minus_three_pt5 );
       CPPUNIT_ASSERT( minus_three_pt5 < three );
	   CPPUNIT_ASSERT( three <= another_three );
	   CPPUNIT_ASSERT( three >= another_three );
	   CPPUNIT_ASSERT( one23pt875 > three );
	   CPPUNIT_ASSERT( one23pt875 >= three );
	   CPPUNIT_ASSERT( three <= one23pt875 );
	}

	/**
	 * Check output to standard stream
	 */
	void test_output()
	{
		Fixed16 twelve(12), one_eigth(1.0/8.0), minus_30(-30);
		std::ostringstream os, os2, os3;

		os << twelve;
		CPPUNIT_ASSERT(os.str() == "12");

		os2 << one_eigth;
		CPPUNIT_ASSERT(os2.str() == "0.125");

		os3 << minus_30;
		CPPUNIT_ASSERT(os3.str() == "-30");
	}

	/**
	 * Check input from a standard stream
	 */
	void test_input()
	{
		std::istringstream is("12 0.125 -30");
		Fixed16 twelve, one_eigth, minus_30;

		is >> twelve >> one_eigth >> minus_30;

		CPPUNIT_ASSERT( twelve == 12 );
		CPPUNIT_ASSERT( one_eigth == 0.125 );
		CPPUNIT_ASSERT( minus_30 == -30 );
	}

	/**
	 * Check addition/subtraction/multiplication/division
	 */
	void test_sums()
	{
		Fixed16 two(2), pt5(0.5), two_pt5(2.5), one_pt5(1.5), minus_one_pt5(-1.5);

		// operator+/- fixed numbers
		CPPUNIT_ASSERT( two + pt5 == two_pt5 );
		CPPUNIT_ASSERT( pt5 + two == two_pt5 );
		CPPUNIT_ASSERT( two - pt5 == one_pt5 );
		CPPUNIT_ASSERT( pt5 - two == minus_one_pt5 );

		// operator+/- fixed and ints
		CPPUNIT_ASSERT( two + 1 == 3 );
		CPPUNIT_ASSERT( 1 + two == 3 );
		CPPUNIT_ASSERT( two_pt5 - 1 == one_pt5 );
		CPPUNIT_ASSERT( 1 - two_pt5 == minus_one_pt5 );

		// operator+/- fixed and double
		CPPUNIT_ASSERT( two + 0.5 == 2.5 );
		CPPUNIT_ASSERT( 0.5 + two == 2.5 );
		CPPUNIT_ASSERT( two - 0.5 == 1.5 );
		CPPUNIT_ASSERT( 0.5 - two == -1.5 );

		// Unary minus quick test
		Fixed16 three(3), minus_three(-three);
		CPPUNIT_ASSERT( minus_three == -3 );
		CPPUNIT_ASSERT( -minus_three == three );

		// operator * and / fixed numbers
		CPPUNIT_ASSERT( two * one_pt5 == three );
		CPPUNIT_ASSERT( one_pt5 * two == three );
		CPPUNIT_ASSERT( two * minus_one_pt5 == minus_three );
		CPPUNIT_ASSERT( minus_one_pt5 * two == minus_three );
		CPPUNIT_ASSERT( three / one_pt5 == two );
		CPPUNIT_ASSERT( three / two == one_pt5 );
		CPPUNIT_ASSERT( minus_three / two == minus_one_pt5 );
		CPPUNIT_ASSERT( minus_three / minus_one_pt5 == two );
		
		// operator * and / fixed and ints
		CPPUNIT_ASSERT( one_pt5 * 2 == three );
		CPPUNIT_ASSERT( 2 * one_pt5 == three );
		CPPUNIT_ASSERT( 10 * two == 20 );
		CPPUNIT_ASSERT( one_pt5 * -2 == minus_three );

		CPPUNIT_ASSERT( three / 2 == one_pt5 );
		CPPUNIT_ASSERT( 4 / two == two );
		CPPUNIT_ASSERT( 6 / three == 2 );
		CPPUNIT_ASSERT( 9 / minus_three == -3 );

		// operator * and / fixed and double
		CPPUNIT_ASSERT( two * 1.5 == three );
		CPPUNIT_ASSERT( 1.5 * two == three );
		CPPUNIT_ASSERT( two * -1.5 == -3.0 );
		CPPUNIT_ASSERT( -1.5 * two == minus_three );
		CPPUNIT_ASSERT( three / 1.5 == two );
		CPPUNIT_ASSERT( 3.0 / one_pt5 == two );
		CPPUNIT_ASSERT( three / -1.5 == -2.0 );
	}

	// Test conversions to ints and doubles
	void test_conversions()
	{
		Fixed16 two(2), minus_three(-3), two_pt5(2.5), minus_one_pt5(-1.5);

		int i2 = two;
		int mi3 = minus_three;
		double d2 = two;
		double d2pt5 = two_pt5;
		double md1pt5 = minus_one_pt5;

		CPPUNIT_ASSERT( i2 == 2);
		CPPUNIT_ASSERT( mi3 == -3);
		CPPUNIT_ASSERT( d2 == two );
		CPPUNIT_ASSERT( d2pt5 == 2.5 );
		CPPUNIT_ASSERT( md1pt5 == -1.5 );

		double diff = two_pt5 - two;
		CPPUNIT_ASSERT(diff == 0.5);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( Fixed16Test );
