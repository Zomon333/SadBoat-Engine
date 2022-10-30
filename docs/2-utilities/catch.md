# Catch
### Prototype  
>```c++
>   TEST_CASE(name, suite) { .../* Do some tests here! */ }
>```
### Usage: Basic  
Catch is a simple, header only library for unit tests. This allows you to test your code in small chunks without learning massive libraries or messing with linker settings on the command line. To get started, you may write a few unit tests and add REQUIRE() and CHECK() statements to ensure your code functions appropriately. Then, you can begin a catch session. The catch session will automatically detect your unit tests, and run them upon first boot so you may test your code. We recommend making a few test cases per feature, and one file per test suite.

### Usage: Advanced  
Despite being a simple library, Catch is still rather flexible and provides many features for developer use. We won't go into full detail here, as we ourselves are also new to Catch, but we do provide a link to Catch's official documentation.

To start the catch session mentioned earlier, you can include this line of code in main: ```int results = Catch::Session().run(argc, argv);```
This indicates to catch that it should begin a session for our program with the same parameters that were passed to the engine. It will return the result of it's unit test to results. This can be helpful for the rest of our program, as you may only want to continue launching if you pass your tests!

### Implementation  
Not Applicable; included as a header only library. Library should not be used in production releases.

### Extra Resources  

[Catch2's Documentation](https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md)

[Last](https://github.com/Zomon333/SadBoat-Engine/blob/Linux-Refactor/docs/1-introduction/table-of-contents.md) --- [Next](https://www.github.com/Zomon333/SadBoat-Engine/tree/Linux-Refactor/docs/2-utilities/rapidxml.md)