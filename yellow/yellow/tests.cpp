//
// Created by mariakolyachko on 19.12.19.
//

#include "tests.h"
#include "bus_manager.h"
//
//void TestPhoneNumber() {
//    {
//        PhoneNumber phoneNumber = PhoneNumber("+375-44-7571398");
//        AssertEqual(phoneNumber.GetInternationalNumber(),
//                    "+375-44-7571398",
//                    "my correct number");
//    }
//
//    {
//        try {
//            PhoneNumber phoneNumber = PhoneNumber("375-44-7571398");
//            Assert(false, "without \'+\'");
//        }
//        catch (std::invalid_argument &ex) {
//            Assert(true, "without \'+\'");
//        }
//        catch (std::exception &ex) {
//            Assert(false, "without \'+\'");
//        }
//    }
//
//    {
//        try {
//            PhoneNumber phoneNumber = PhoneNumber("+-375-44-7571398");
//            Assert(false, "without \'+\'");
//        }
//        catch (std::invalid_argument &ex) {
//            Assert(true, "without \'+\'");
//        }
//        catch (std::exception &ex) {
//            Assert(false, "without \'+\'");
//        }
//    }
//
//    {
//        try {
//            PhoneNumber phoneNumber = PhoneNumber("++375-44-7571398");
//            Assert(phoneNumber.GetInternationalNumber() == "++375-44-7571398",
//                   "");
//        }
//        catch (std::invalid_argument &ex) {
//            Assert(false, "without \'+\'");
//        }
//        catch (std::exception &ex) {
//            Assert(false, "without \'+\'");
//        }
//    }
//}
//
//void TestAll() {
//    TestRunner tr;
//    tr.RunTest(TestPhoneNumber, "TestPhoneNumber");
//}
//
