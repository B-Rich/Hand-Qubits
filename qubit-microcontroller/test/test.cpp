#include <stdio.h>
#include <exception>
#include <string>
#include "../quaternion.h"
#include "../quaternion.cpp"
#define PI 3.14159265358979323846

static std::string cur_name = "";
static int cur_assert = 0;

void test(std::string name, void(*func)(void)) {
	cur_assert = 0;
	cur_name = name;
	func();
}

void assert(bool truth) {
	cur_assert++;
	if (!truth) {
		throw "assert in " + cur_name + " at #" + std::to_string(cur_assert);
	}
}

void assert_near(Quaternion q1, Quaternion q2) {
	auto qd = q1 - q2;
	auto d = qd.w*qd.w + qd.x*qd.x + qd.y*qd.y + qd.z*qd.z;
	assert(d < 0.0001);
}

int main() {
	test("quaternion_equality", [](){
		constexpr Quaternion q{ 2, 3, 5, 7 };

		assert(q == Quaternion{ 2, 3, 5, 7 });
		assert(!(q == Quaternion{ 0, 3, 5, 7 }));
		assert(!(q == Quaternion{ 2, 0, 5, 7 }));
		assert(!(q == Quaternion{ 2, 3, 0, 7 }));
		assert(!(q == Quaternion{ 2, 3, 5, 0 }));
	});

	test("quaternion_addition", []() {
		assert(Quaternion{ 2, 3, 5, 7 } + Quaternion{ 11, 13, 17, 19 } == Quaternion{ 13, 16, 22, 26 });
	});

	test("quaternion_subtraction", []() {
		assert(Quaternion{ 2, 3, 5, 7 } - Quaternion{ 11, 13, 17, 19 } == Quaternion{ -9, -10, -12, -12 });
	});

	test("quaternion_multiplication", []() {
		assert(Quaternion{ 2, 3, 5, 7 } * Quaternion{ 11, 13, 17, 19 } == Quaternion{ -235, 35, 123, 101 });
	});

	test("quaternion_conj", []() {
		assert(Quaternion{ 2, 3, 5, 7 }.conj() == Quaternion{ 2, -3, -5, -7 });
	});

	test("quaternion_from_angular_velocity", []() {
		assert(Quaternion::from_angular_impulse(0, 0, 0) == Quaternion{ 1, 0, 0, 0 });
		assert_near(Quaternion::from_angular_impulse(2*PI, 0, 0), Quaternion{ -1, 0, 0, 0 });

		assert_near(Quaternion::from_angular_impulse(PI, 0, 0), Quaternion{ 0, 1, 0, 0 });
		assert_near(Quaternion::from_angular_impulse(PI/2, 0, 0), Quaternion{ sqrtf(0.5), sqrtf(0.5), 0, 0 });
	});

	printf("PASS");
	return 0;
}
