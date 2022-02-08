#include "pch.h"
#include "CppUnitTest.h"
#include "../TheEventLoopOfLife/src/Subject.h"
#include "../TheEventLoopOfLife/src/Observer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheEventLoopOfLifeTests
{
	TEST_CLASS(TheEventLoopOfLifeTests)
	{
	public:
		
		TEST_METHOD(TestRemoveObserver)
		{
			Observer *grass = new Observer();
			Observer *wolf = new Observer();

			Subject* sheepAte = new Subject();
			sheepAte->addObserver(grass);
			sheepAte->addObserver(wolf);
			sheepAte->removeObserver(grass);

			Subject* expected = new Subject();
			expected->addObserver(wolf);


			Assert::AreEqual(*expected, *sheepAte);
		}
	};
}
