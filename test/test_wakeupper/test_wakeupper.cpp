#include <unity.h>
#include <chrono>
#include <thread>
#include "../src/Services/Wakeupper/Wakeupper.cpp"

Wakeupper wakeupper(5);

void test_ready_not_ready_for_wakeup(void) {
    TEST_ASSERT_EQUAL(false, wakeupper.isWakeupTime());
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    TEST_ASSERT_EQUAL(false, wakeupper.isWakeupTime());
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    TEST_ASSERT_EQUAL(true, wakeupper.isWakeupTime());
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    TEST_ASSERT_EQUAL(false, wakeupper.isWakeupTime());
    std::this_thread::sleep_for(std::chrono::milliseconds(4));
    TEST_ASSERT_EQUAL(true, wakeupper.isWakeupTime());
}

int main( int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_ready_not_ready_for_wakeup);
    UNITY_END();
};