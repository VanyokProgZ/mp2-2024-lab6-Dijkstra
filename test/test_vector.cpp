//тесты для самописного вектора
#include <gtest.h>
#include "vector.h"

TEST(vector, can_create_vector_with_positive_length)
{
	ASSERT_NO_THROW(vector<int> v(5));
}
TEST(vector, can_create_vector_with_zero_length)
{
	ASSERT_NO_THROW(vector<int> v(0));
}
TEST(vector, can_use_size_method) {
	vector<int> v(1);
	ASSERT_NO_THROW(v.size());
}
TEST(vector, right_size_after_create)
{
	vector<int> v(12);
	ASSERT_EQ(v.size(),12);
}
TEST(vector, can_access_to_memory)
{
	vector<int> v(10);
	ASSERT_NO_THROW(v.at(2)=3);
}
TEST(vector, can_access_to_back)
{
	vector<int> v(10);
	ASSERT_NO_THROW(v.back() = 3);
}
TEST(vector, can_access_to_back_correct)
{
	vector<int> v(10);
	v.back() = 115;
	ASSERT_EQ(115, v.at(9));
}
TEST(vector, cant_access_to_neg_memory)
{
	vector<int> v(10);
	ASSERT_ANY_THROW(v.at(-2) = 3);
}
TEST(vector, cant_access_to_up_memory)
{
	vector<int> v(10);
	ASSERT_ANY_THROW(v.at(10) = 3);
}
TEST(vector, right_set_and_get_elements)
{
	vector<int> v(10);
	v.at(0) = 234;
	ASSERT_EQ(v.at(0), 234);
}
TEST(vector, can_use_initializer_list)
{
	std::initializer_list<int> l = { 1,1,2,3,5,8 };
	ASSERT_NO_THROW( vector<int> v = l );
}
TEST(vector, can_use_initializer_list_with_null_size)
{
	std::initializer_list<int> l = {};
	ASSERT_NO_THROW(vector<int> v = l);
}
TEST(vector, right_elements_from_initializer_list)
{
	vector<int> v = {1,2,3,4,5};
	ASSERT_EQ(v.at(0) == 1 && v.at(1) == 2 && v.at(2) == 3 && v.at(3) == 4 && v.at(4) == 5, true);
}
TEST(vector, can_create_from_another_vector)
{
	vector<int> v1(1);
	ASSERT_NO_THROW(vector<int> v2 = v1);
}
TEST(vector, create_from_another_vector_set_size)
{
	vector<int> v1(1);
	vector<int> v2 = v1;
	ASSERT_EQ(v1.size(), v2.size());
}
TEST(vector, create_from_another_vector_own_mem)
{
	vector<int> v1(2);
	v1.at(0) = 1;
	v1.at(1) = 2;
	vector<int> v2 = v1;
	v2.at(0) = 3;
	ASSERT_EQ(v1.at(0)==1 && v1.at(1)==2 && v1.at(1)==v2.at(1) && v2.at(0)==3, true);
}
TEST(vector, can_create_and_fill)
{
	ASSERT_NO_THROW(vector<int> v1(2, 3));
	vector<int> v1(2, 3);
	ASSERT_EQ(v1.at(0) == 3 && v1.at(1), true);
}
TEST(vector, can_use_capacity)
{
	vector<int> v1(2, 3);
	ASSERT_NO_THROW(v1.capacity());
}
TEST(vector, create_capacity_eq_size_plus_one)
{
	vector<int> v1(2, 3);
	ASSERT_EQ(v1.size()+1,v1.capacity());
}
TEST(vector, can_push_back)
{
	vector<int> v1(2, 3);
	ASSERT_NO_THROW(v1.push_back(5));
	ASSERT_EQ(v1.back(), 5);
}
TEST(vector, push_back_change_size)
{
	vector<int> v1(2, 3);
	v1.push_back(5);
	ASSERT_EQ(v1.size(), 3);
}
TEST(vector, push_back_realloc_mem)
{
	vector<int> v1(2, 3);
	v1.push_back(5);
	ASSERT_NO_THROW(v1.push_back(6));
	ASSERT_EQ(v1.capacity(), 6);
}
TEST(vector, can_pop_back)
{
	vector<int> v1(2, 3);
	ASSERT_NO_THROW(v1.pop_back());
	ASSERT_EQ(v1.size(), 1);
}
TEST(vector, cant_pop_back_empry_vector)
{
	vector<int> v1(0);
	ASSERT_ANY_THROW(v1.pop_back());
}
TEST(vector, assign_operator)
{
	vector<int> v1(4,5);
	vector<int> v2(3, 2);
	ASSERT_NO_THROW(v1=v2);
	ASSERT_EQ(v1.at(0)==2 && v1.at(1)==2 && v1.at(2)==2,true);
}
TEST(vector, assign_operator_change_size)
{
	vector<int> v1(11, 5);
	vector<int> v2(3, 2);
	v2 = v1;
	ASSERT_EQ(v2.size(),11);
}
TEST(vector, assign_operator_change_capacity)
{
	vector<int> v1(11, 5);
	vector<int> v2(3, 2);
	v2 = v1;
	ASSERT_EQ(v2.capacity(), 12);
}
TEST(vector, can_resize)
{
	vector<int> v1(11, 5);
	ASSERT_NO_THROW(v1.resize(6));
	ASSERT_EQ(v1.size(),6);
}
TEST(vector, can_clear)
{
	vector<int> v1(11, 5);
	ASSERT_NO_THROW(v1.clear());
	ASSERT_EQ(v1.size(), 0);
}
TEST(vector, can_move_constructor)
{
	vector<int> v1 = { 1,2,3,4,5 };
	ASSERT_NO_THROW(vector<int> v2(std::move(v1)));
}
TEST(vector, true_size_v2_move_constructor)
{
	vector<int> v1 = { 1,2,3,4,5 };
	vector<int> v2(std::move(v1));
	ASSERT_EQ(v2.size(), 5);
}
TEST(vector, true_cp_v2_move_constructor)
{
	vector<int> v1 = { 1,2,3,4,5 };
	size_t cp = v1.capacity();
	vector<int> v2(std::move(v1));
	ASSERT_EQ(v2.capacity(), cp);
}
TEST(vector, true_data_v2_move_constructor)
{
	vector<int> v1 = { 1,2,3,4,5 };
	vector<int> v2(std::move(v1));
	bool f = 1;
	for (int i = 1; i <= 5; i++) {
		if (v2[i - 1] != i) f = 0;
	}
	ASSERT_EQ(f, 1);
}
TEST(vector, true_size_v1_move_constructor)
{
	vector<int> v1 = { 1,2,3,4,5 };
	vector<int> v2(std::move(v1));
	ASSERT_EQ(v1.size(), 0);
}
TEST(vector, true_cp_v1_move_constructor)
{
	vector<int> v1 = { 1,2,3,4,5 };
	vector<int> v2(std::move(v1));
	ASSERT_EQ(v1.capacity(), 0);
}
TEST(vector, resize_correct_data)
{
	vector<int> v1 = { 1,2,3,4,5 };
	v1.resize(3);
	ASSERT_EQ(v1[2],3);
}
TEST(vector, can_move_assign)
{
	vector<int> v2 = { 1,2,3,4,5 };
	vector<int> v1 = {9,8,7};
	ASSERT_NO_THROW(v1 = std::move(v2));
}
TEST(vector, move_assign_v1_right_size_)
{
	vector<int> v2 = { 1,2,3,4,5 };
	vector<int> v1 = { 9,8,7 };
	v1 = std::move(v2);
	ASSERT_EQ(v1.size(), 5);
}
TEST(vector, move_assign_v2_right_size_)
{
	vector<int> v2 = { 1,2,3,4,5 };
	vector<int> v1 = { 9,8,7 };
	v1 = std::move(v2);
	ASSERT_EQ(v2.size(), 0);
}
TEST(vector, move_assign_v2_right_cp_)
{
	vector<int> v2 = { 1,2,3,4,5 };
	vector<int> v1 = { 9,8,7 };
	v1 = std::move(v2);
	ASSERT_EQ(v2.capacity(), 0);
}
TEST(vector, move_assign_v1_right_cp_)
{
	vector<int> v2 = { 1,2,3,4,5 };
	size_t cp = v2.capacity();
	vector<int> v1 = { 9,8,7 };
	v1 = std::move(v2);
	ASSERT_EQ(v1.capacity(), cp);
}
TEST(vector, right_mem_move_assign)
{
	vector<int> v2 = { 1,2,3,4,5 };
	vector<int> v1 = { 9,8,7 };
	v1 = std::move(v2);
	ASSERT_EQ(v1[0],1);
	ASSERT_EQ(v1[1], 2);
	ASSERT_EQ(v1[2], 3);
	ASSERT_EQ(v1[3], 4);
	ASSERT_EQ(v1[4], 5);
}