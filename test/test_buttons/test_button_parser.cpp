#include <unity.h>
#include <string>
#include <../src/Devices/Buttons/ButtonParser/ButtonParser.cpp>

ButtonParser buttonParser;

void setUp(void) {
// set stuff up here
}

void tearDown(void) {
// clean stuff up here
}

void test_no_button_pressed(void) {
    TEST_ASSERT(ButtonParser::NO_BUTTON == buttonParser.buttonCode(100));
}

void test_down_button_pressed(void) {
    TEST_ASSERT(ButtonParser::DOWN_BUTTON == buttonParser.buttonCode(650));
}

void test_select_button_pressed(void) {
    TEST_ASSERT(ButtonParser::SELECT_BUTTON == buttonParser.buttonCode(750));
}

void test_up_button_pressed(void) {
    TEST_ASSERT(ButtonParser::UP_BUTTON == buttonParser.buttonCode(850));
}

void test_deploy_button_pressed(void) {
    TEST_ASSERT(ButtonParser::DEPLOY_BUTTON == buttonParser.buttonCode(950));
}

int main( int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_no_button_pressed);
    RUN_TEST(test_down_button_pressed);
    RUN_TEST(test_up_button_pressed);
    RUN_TEST(test_select_button_pressed);
    RUN_TEST(test_deploy_button_pressed);
    UNITY_END();
};