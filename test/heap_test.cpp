#include <gtest.h>
#include "heap.h"

TEST(b_heap, can_push) {
	heap<int, std::less<int>> h;
	ASSERT_NO_THROW(h.push(12));
}
TEST(b_heap, can_push_same) {
	heap<int, std::less<int>> h;
	h.push(12);
	ASSERT_NO_THROW(h.push(12));
}
TEST(b_heap, can_top) {
	heap<int, std::less<int>> h;
	h.push(12);
	ASSERT_NO_THROW(h.top());
}
TEST(b_heap, top_correct) {
	heap<int, std::less<int>> h;
	h.push(12);
	ASSERT_EQ(12, h.top());
}
TEST(b_heap, correct) {
	heap<int, std::less<int>> h;
	h.push(12);
	h.push(123);
	h.push(1);
	h.push(4);
	ASSERT_EQ(1, h.top());
	h.extract();
	ASSERT_EQ(4, h.top());
	h.extract();
	ASSERT_EQ(12, h.top());
	h.extract();
	ASSERT_EQ(123, h.top());
}
TEST(b_heap,cant_top_empty) {
	heap<int, std::less<int>> h;
	ASSERT_ANY_THROW(h.top());
}
TEST(b_heap, cant_extract_empty) {
	heap<int, std::less<int>> h;
	ASSERT_ANY_THROW(h.extract());
}
TEST(b_heap, empty_yes) {
	heap<int, std::less<int>> h;
	ASSERT_EQ(1, h.empty());
}
TEST(b_heap, empty_no) {
	heap<int, std::less<int>> h;
	h.push(23);
	ASSERT_EQ(0, h.empty());
}




TEST(bin_heap, can_push) {
	BinomialHeap<int, std::less<int>> h;
	ASSERT_NO_THROW(h.push(12));
}
TEST(bin_heap, can_push_same) {
	BinomialHeap<int, std::less<int>> h;
	h.push(12);
	ASSERT_NO_THROW(h.push(12));
}
TEST(bin_heap, can_top) {
	BinomialHeap<int, std::less<int>> h;
	h.push(12);
	ASSERT_NO_THROW(h.top());
}
TEST(bin_heap, top_correct) {
	BinomialHeap<int, std::less<int>> h;
	h.push(12);
	ASSERT_EQ(12, h.top());
}
TEST(bin_heap, correct) {
	BinomialHeap<int, std::less<int>> h;
	h.push(12);
	h.push(123);
	h.push(1);
	h.push(4);
	ASSERT_EQ(1, h.top());
	h.extract();
	ASSERT_EQ(4, h.top());
	h.extract();
	ASSERT_EQ(12, h.top());
	h.extract();
	ASSERT_EQ(123, h.top());
}
TEST(bin_heap, cant_top_empty) {
	BinomialHeap<int, std::less<int>> h;
	ASSERT_ANY_THROW(h.top());
}
TEST(bin_heap, cant_extract_empty) {
	BinomialHeap<int, std::less<int>> h;
	ASSERT_ANY_THROW(h.extract());
}
TEST(bin_heap, empty_yes) {
	BinomialHeap<int, std::less<int>> h;
	ASSERT_EQ(1, h.empty());
}
TEST(bin_heap, empty_no) {
	BinomialHeap<int, std::less<int>> h;
	h.push(23);
	ASSERT_EQ(0, h.empty());
}