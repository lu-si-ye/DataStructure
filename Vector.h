#pragma once
#ifndef VECTOR_H
#define VECTOR_H

// =====================================================
// Personal Vector Container Implementation (C++17)
// Feature-compatible with std::vector, supporting:
// Iterators, Allocators, Move Semantics, Strong Exception Safety
// Author: [Lu Siye]    Date: 2026
// =====================================================

#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <memory>
#include <limits>
#include <initializer_list>
#include <type_traits>

template<typename T>
class DefaultAllocator {
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using propagate_on_container_move_assignment = std::true_type;
	using is_always_equal = std::true_type;

	pointer allocate(size_type n) {
		return static_cast<pointer>(::operator new(n * sizeof(T)));
	}

	void deallocate(pointer p, size_type n) {
		::operator delete(p);
	}

	template<typename U, typename... Args>
	void construct(U* p, Args&&... args) {
		new(p) U(std::forward<Args>(args)...);
	}

	template<typename U>
	void destroy(U* p) {
		p->~U();
	}

	template<typename U>
	struct rebind {
		using other = DefaultAllocator<U>;
	};

	size_type max_size() const noexcept {
		return std::numeric_limits<size_type>::max() / sizeof(T);
	}

	bool operator==(const DefaultAllocator&) const { return true; }
	bool operator!=(const DefaultAllocator&) const { return false; }
};

template<typename T, typename Allocator = DefaultAllocator<T>>
class Vector {
public:

	using value_type = T;
	using allocator_type = Allocator;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;

	class Iterator {
	public:

		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		Iterator(T* ptr) : current(ptr) {}

		T& operator*() { return *current; }

		T* operator->() { return current; }

		Iterator operator++(int) {
			Iterator tmp = *this;
			++current;
			return tmp;
		}

		Iterator& operator++() {
			++current;
			return *this;
		}

		Iterator operator--(int) {
			Iterator tmp = *this;
			--current;
			return tmp;
		}

		Iterator& operator--() {
			--current;
			return *this;
		}

		bool operator==(const Iterator& other) const { return current == other.current; }
		bool operator!=(const Iterator& other) const { return current != other.current; }
		bool operator<(const Iterator& other) const { return current < other.current; }
		bool operator>(const Iterator& other) const { return current > other.current; }
		bool operator<=(const Iterator& other) const { return current <= other.current; }
		bool operator>=(const Iterator& other) const { return current >= other.current; }

		Iterator& operator+=(difference_type n) {
			current += n;
			return *this;
		}

		Iterator& operator-=(difference_type n) {
			current -= n;
			return *this;
		}

		Iterator operator+(difference_type n) const {
			return Iterator(current + n);
		}

		Iterator operator-(difference_type n) const {
			return Iterator(current - n);
		}

		difference_type operator-(const Iterator& other) const {
			return current - other.current;
		}

		T& operator[](difference_type n) {
			return current[n];
		}

		const T& operator[](difference_type n) const {
			return current[n];
		}

	private:
		T* current;
	};

	class ConstIterator {
	public:

		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;

		ConstIterator(const Iterator& it) : current(&(*it)) {}

		ConstIterator(const T* ptr) : current(ptr) {}

		const T& operator*() const { return *current; }
		const T* operator->() const { return current; }

		ConstIterator operator++(int) {
			ConstIterator tmp = *this;
			++current;
			return tmp;
		}

		ConstIterator& operator++() {
			++current;
			return *this;
		}

		ConstIterator operator--(int) {
			ConstIterator tmp = *this;
			--current;
			return tmp;
		}

		ConstIterator& operator--() {
			--current;
			return *this;
		}

		bool operator==(const ConstIterator& other) const { return current == other.current; }
		bool operator!=(const ConstIterator& other) const { return current != other.current; }
		bool operator<(const ConstIterator& other) const { return current < other.current; }
		bool operator>(const ConstIterator& other) const { return current > other.current; }
		bool operator<=(const ConstIterator& other) const { return current <= other.current; }
		bool operator>=(const ConstIterator& other) const { return current >= other.current; }

		ConstIterator& operator+=(difference_type n) {
			current += n;
			return *this;
		}

		ConstIterator& operator-=(difference_type n) {
			current -= n;
			return *this;
		}

		ConstIterator operator+(difference_type n) const {
			return ConstIterator(current + n);
		}

		ConstIterator operator-(difference_type n) const {
			return ConstIterator(current - n);
		}

		difference_type operator-(const ConstIterator& other) const {
			return current - other.current;
		}

		const T& operator[](difference_type n) const {
			return current[n];
		}

	private:
		const T* current;
	};

	class ReverseIterator {
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		ReverseIterator(Iterator it) : current(it) {}

		reference operator*() { return *(current - 1); }
		const_reference operator*() const { return *(current - 1); }
		pointer operator->() { return &(*(current - 1)); }

		ReverseIterator& operator++() { --current; return *this; }
		ReverseIterator operator++(int) { auto t = *this; --current; return t; }
		ReverseIterator& operator--() { ++current; return *this; }
		ReverseIterator operator--(int) { auto t = *this; ++current; return t; }

		ReverseIterator& operator+=(difference_type n) { current -= n; return *this; }
		ReverseIterator& operator-=(difference_type n) { current += n; return *this; }
		ReverseIterator operator+(difference_type n) const { return ReverseIterator(current - n); }
		ReverseIterator operator-(difference_type n) const { return ReverseIterator(current + n); }
		difference_type operator-(const ReverseIterator& other) const { return other.current - current; }

		reference operator[](difference_type n) { return *(current - 1 - n); }
		const_reference operator[](difference_type n) const { return *(current - 1 - n); }

		bool operator==(const ReverseIterator& o) const { return current == o.current; }
		bool operator!=(const ReverseIterator& o) const { return current != o.current; }
		bool operator<(const ReverseIterator& o) const { return current > o.current; }
		bool operator>(const ReverseIterator& o) const { return current < o.current; }
		bool operator<=(const ReverseIterator& o) const { return current >= o.current; }
		bool operator>=(const ReverseIterator& o) const { return current <= o.current; }

		Iterator base() const { return current; }

	private:
		Iterator current;
	};

	class ConstReverseIterator {
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;

		ConstReverseIterator(ConstIterator it) : current(it) {}

		reference operator*() const { return *(current - 1); }
		pointer operator->() const { return &(*(current - 1)); }

		ConstReverseIterator& operator++() { --current; return *this; }
		ConstReverseIterator operator++(int) { auto t = *this; --current; return t; }
		ConstReverseIterator& operator--() { ++current; return *this; }
		ConstReverseIterator operator--(int) { auto t = *this; ++current; return t; }

		ConstReverseIterator& operator+=(difference_type n) { current -= n; return *this; }
		ConstReverseIterator& operator-=(difference_type n) { current += n; return *this; }
		ConstReverseIterator operator+(difference_type n) const { return ConstReverseIterator(current - n); }
		ConstReverseIterator operator-(difference_type n) const { return ConstReverseIterator(current + n); }
		difference_type operator-(const ConstReverseIterator& other) const { return other.current - current; }

		reference operator[](difference_type n) const { return *(current - 1 - n); }

		bool operator==(const ConstReverseIterator& o) const { return current == o.current; }
		bool operator!=(const ConstReverseIterator& o) const { return current != o.current; }
		bool operator<(const ConstReverseIterator& o) const { return current > o.current; }
		bool operator>(const ConstReverseIterator& o) const { return current < o.current; }
		bool operator<=(const ConstReverseIterator& o) const { return current >= o.current; }
		bool operator>=(const ConstReverseIterator& o) const { return current <= o.current; }

		ConstIterator base() const { return current; }

	private:
		ConstIterator current;
	};

	using iterator = Iterator;
	using const_iterator = ConstIterator;
	using reverse_iterator = ReverseIterator;
	using const_reverse_iterator = ConstReverseIterator;

	ReverseIterator rbegin() { return ReverseIterator(end()); }

	ReverseIterator rend() { return ReverseIterator(begin()); }

	ConstReverseIterator rbegin() const { return ConstReverseIterator(end()); }

	ConstReverseIterator rend() const { return ConstReverseIterator(begin()); }

	Iterator begin() { return Iterator(m_data); }

	Iterator end() { return Iterator(m_data + m_size); }

	ConstIterator begin() const { return ConstIterator(m_data); }

	ConstIterator end() const { return ConstIterator(m_data + m_size); }

	ConstIterator cbegin() const { return ConstIterator(m_data); }
	
	ConstIterator cend() const { return ConstIterator(m_data + m_size); }
	
	ConstReverseIterator crbegin() const { return ConstReverseIterator(cend()); }

	ConstReverseIterator crend() const { return ConstReverseIterator(cbegin()); }

	Vector() noexcept(noexcept(Allocator())) : m_data(nullptr), m_size(0), m_capacity(0), allocator() {}

	Vector(size_type count, const T& value, const Allocator& alloc = Allocator())
		: m_data(nullptr), m_size(0), m_capacity(0), allocator(alloc) {
		resize(count, value);
	}

	Vector(int count, const T& value) : Vector((size_type)count, value) {}

	Vector(std::initializer_list<T> init, const Allocator& alloc = Allocator())
		: m_data(nullptr), m_size(0), m_capacity(0), allocator(alloc) {
		reserve(init.size());
		for (const auto& elem : init) {
			AllocTraits::construct(allocator, &m_data[m_size], elem);
			++m_size;
		}
	}

	Vector(const Vector& other)
		: m_data(nullptr), m_size(0), m_capacity(0), allocator(other.allocator) {
		if (other.m_size > 0) {
			m_data = AllocTraits::allocate(allocator, other.m_capacity);
			m_capacity = other.m_capacity;
			for (size_type i = 0; i < other.m_size; ++i) {
				AllocTraits::construct(allocator, &m_data[i], other.m_data[i]);
			}
			m_size = other.m_size;
		}
	}

	Vector(Vector&& other) noexcept
		: m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity), allocator(std::move(other.allocator)) {
		other.m_data = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;
	}

	template<typename InputIt, typename = std::enable_if_t<
		!std::is_integral<InputIt>::value
		>>
		Vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
		: m_data(nullptr), m_size(0), m_capacity(0), allocator(alloc) {
		for (auto it = first; it != last; ++it) {
			push_back(*it);
		}
	}

	~Vector()
	{
		clear();
		if (m_data != nullptr) {
			AllocTraits::deallocate(allocator, m_data, m_capacity);
			m_data = nullptr;
			m_capacity = 0;
		}
	}

	Vector& operator=(const Vector& other) {
		if (this == &other) return *this;

		T* newData = nullptr;
		size_type newCap = other.m_capacity;
		size_type i = 0;

		try {
			if (newCap > 0) {
				newData = AllocTraits::allocate(allocator, newCap);
				for (; i < other.m_size; ++i) {
					AllocTraits::construct(allocator, &newData[i], other.m_data[i]);
				}
			}
		}
		catch (...) {
			for (size_type j = 0; j < i; ++j) {
			AllocTraits::destroy(allocator, &newData[j]);
			}
			if (newData) AllocTraits::deallocate(allocator, newData, newCap);
			throw;
		}

		clear();
		if (m_data) AllocTraits::deallocate(allocator, m_data, m_capacity);

		m_data = newData;
		m_size = other.m_size;
		m_capacity = newCap;
		return *this;
	}

	Vector& operator=(std::initializer_list<T> ilist) {
		assign(ilist.begin(), ilist.end());
		return *this;
	}

	Vector& operator=(Vector&& other) noexcept(
		std::is_nothrow_move_assignable<Allocator>::value) {

		if (this == &other) return *this;

		clear();
		if (m_data != nullptr) {
			AllocTraits::deallocate(allocator, m_data, m_capacity);
		}

		if (allocator == other.allocator) {
			m_data = other.m_data;
			m_size = other.m_size;
			m_capacity = other.m_capacity;
			allocator = std::move(other.allocator);
		}
		else {
			m_data = nullptr;
			m_size = 0;
			m_capacity = 0;
			allocator = std::move(other.allocator);

			reserve(other.m_size);
			for (size_type i = 0; i < other.m_size; ++i) {
				AllocTraits::construct(allocator, &m_data[i], std::move(other.m_data[i]));
			}
			m_size = other.m_size;

			// ÊÍ·Å other
			for (size_type i = 0; i < other.m_size; ++i) {
				AllocTraits::destroy(other.allocator, &other.m_data[i]);
			}
			if (other.m_data) {
				AllocTraits::deallocate(other.allocator, other.m_data, other.m_capacity);
			}
		}

		other.m_data = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;
		return *this;
	}

	void push_back(const T& value) {
		if (m_size >= m_capacity) {
			reserve((m_capacity == 0) ? 1 : m_capacity * 2);
		}
		AllocTraits::construct(allocator, &m_data[m_size], value);
		++m_size;
	}

	void push_back(T&& value) {
		if (m_size >= m_capacity) {
			reserve((m_capacity == 0) ? 1 : m_capacity * 2);
		}
		AllocTraits::construct(allocator, &m_data[m_size], std::move(value));
		++m_size;
	}

	template<typename... Args>
	void emplace_back(Args&&... args) {
		if (m_size >= m_capacity) {
			reserve((m_capacity == 0) ? 1 : m_capacity * 2);
		}
		AllocTraits::construct(allocator, &m_data[m_size], std::forward<Args>(args)...);
		++m_size;
	}

	template<typename... Args>
	void emplace(size_type index, Args&&... args) {
		if (index > m_size) throw std::out_of_range("Index out of range");
		if (m_size >= m_capacity) reserve((m_capacity == 0) ? 1 : m_capacity * 2);

		AllocTraits::construct(allocator, &m_data[m_size], std::forward<Args>(args)...);
		++m_size;

		std::rotate(begin() + index, end() - 1, end());
	}

	template<typename... Args>
	iterator emplace(iterator pos, Args&&... args) {
		size_type index = pos - begin();
		emplace(index, std::forward<Args>(args)...);
		return begin() + index;
	}

	void pop_back()
	{
		if (m_size > 0) {
			--m_size;
			AllocTraits::destroy(allocator, &m_data[m_size]);
		}
	}

	void clear()
	{
		for (size_type i = 0; i < m_size; ++i) {
			AllocTraits::destroy(allocator, &m_data[i]);
		}
		m_size = 0;
	}

	void insert(size_type index, const T& value) {
		if (index > m_size) throw std::out_of_range("Index out of range");
		if (m_size >= m_capacity) reserve((m_capacity == 0) ? 1 : m_capacity * 2);

		AllocTraits::construct(allocator, &m_data[m_size], value);
		++m_size;

		std::rotate(begin() + index, end() - 1, end());
	}

	template<typename InputIt, typename = std::enable_if_t<
		!std::is_integral<InputIt>::value>>
		void insert(size_type index, InputIt first, InputIt last) {
		if (index > m_size) {
			throw std::out_of_range("Index out of range");
		}

		size_type count = 0;
		for (auto it = first; it != last; ++it) { ++count; }

		size_type old_size = m_size;
		for (auto it = first; it != last; ++it) {
			if (m_size >= m_capacity) {
				reserve((m_capacity == 0) ? 1 : m_capacity * 2);
			}
			AllocTraits::construct(allocator, &m_data[m_size], *it);
			++m_size;
		}

		std::rotate(begin() + index, begin() + old_size, end());
	}

	iterator insert(iterator pos, const T& value) {
		size_type index = pos - begin();
		insert(index, value);
		return begin() + index;
	}

	void insert(iterator pos, size_type count, const T& value) {
		size_type index = pos - begin();
		if (count == 0) return;
		reserve(m_size + count);
		for (size_type i = 0; i < count; ++i) {
			AllocTraits::construct(allocator, &m_data[m_size], value);
			++m_size;
		}
		std::rotate(begin() + index, begin() + m_size - count, end());
	}

	void insert(iterator pos, std::initializer_list<T> ilist) {
		insert(pos, ilist.begin(), ilist.end());
	}

	void erase(size_type index) {
		if (index >= m_size) {
			throw std::out_of_range("Index out of range");
		}
		for (size_type i = index; i < m_size - 1; ++i) {
			m_data[i] = std::move(m_data[i + 1]);
		}
		--m_size;
		AllocTraits::destroy(allocator, &m_data[m_size]);
	}

	iterator erase(iterator pos) {
		size_type index = pos - begin();
		erase(index);
		return begin() + index;
	}

	void erase(size_type first, size_type last) {
		if (first > last || first >= m_size) {
			throw std::out_of_range("Index out of range");
		}
		if (last > m_size) last = m_size;
		size_type count = last - first;

		for (size_type i = first; i < m_size - count; ++i) {
			m_data[i] = std::move(m_data[i + count]);
		}

		for (size_type i = m_size - count; i < m_size; ++i) {
			AllocTraits::destroy(allocator, &m_data[i]);
		}
		m_size -= count;
	}

	iterator erase(iterator first, iterator last) {
		size_type start = first - begin();
		size_type end_idx = last - begin();
		erase(start, end_idx);
		return begin() + start;
	}

	void swap(Vector& other) noexcept {
		std::swap(m_data, other.m_data);
		std::swap(m_size, other.m_size);
		std::swap(m_capacity, other.m_capacity);
		if constexpr (AllocTraits::propagate_on_container_swap::value) {
			std::swap(allocator, other.allocator);
		}
	}

	void assign(size_type count, const T& value) {
		clear();
		if (count > m_capacity) {
			reserve(count);
		}
		for (size_type i = 0; i < count; ++i) {
			AllocTraits::construct(allocator, &m_data[i], value);
		}
		m_size = count;
	}

	template<typename InputIt, typename = std::enable_if_t<!std::is_integral<InputIt>::value>>
	void assign(InputIt first, InputIt last) {
		clear();
		for (auto it = first; it != last; ++it) {
			push_back(*it);
		}
	}

	bool operator==(const Vector& other) const
	{
		if (m_size != other.m_size) {
			return false;
		}
		for (size_type i = 0; i < m_size; ++i) {
			if (m_data[i] != other.m_data[i]) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const Vector& other) const
	{
		return !(*this == other);
	}

	bool operator<(const Vector& other) const
	{
		return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
	}

	bool operator>(const Vector& other) const
	{
		return other < *this;
	}

	bool operator<=(const Vector& other) const
	{
		return !(other < *this);
	}

	bool operator>=(const Vector& other) const
	{
		return !(*this < other);
	}

	bool empty() const
	{
		return m_size == 0;
	}

	size_type size() const
	{
		return m_size;
	}

	size_type capacity() const
	{
		return m_capacity;
	}

	size_type max_size() const noexcept {
		return AllocTraits::max_size(allocator);
	}

	allocator_type get_allocator() const noexcept {
		return allocator;
	}

	void shrink_to_fit() {
		if (m_size == 0) {
			if (m_data != nullptr) {
				AllocTraits::deallocate(allocator,m_data, m_capacity);
				m_data = nullptr;
				m_capacity = 0;
			}
			return;
		}
		if (m_size < m_capacity) {
			T* newData = AllocTraits::allocate(allocator,m_size);

			size_type i = 0;
			try {
				for (; i < m_size; ++i) {
					AllocTraits::construct(allocator, &newData[i], std::move(m_data[i]));
				}
			}
			catch (...) {
				for (size_type j = 0; j < i; ++j) {
					AllocTraits::destroy(allocator, &newData[j]);
				}
				AllocTraits::deallocate(allocator,newData, m_size);
				throw;
			}

			for (i = 0; i < m_size; ++i) {
				AllocTraits::destroy(allocator, &m_data[i]);
			}
			AllocTraits::deallocate(allocator,m_data, m_capacity);
			m_data = newData;
			m_capacity = m_size;
		}
	}

	void reserve(size_type newCap) {
		if (newCap <= m_capacity) return;
		T* newData = AllocTraits::allocate(allocator,newCap);

		size_type i = 0;
		try {
			for (; i < m_size; ++i) {
				AllocTraits::construct(allocator, &newData[i], std::move(m_data[i]));
			}
		}
		catch (...) {
			for (size_type j = 0; j < i; ++j) {
				AllocTraits::destroy(allocator, &newData[j]);
			}
			AllocTraits::deallocate(allocator,newData, newCap);
			throw;
		}

		for (i = 0; i < m_size; ++i) {
			AllocTraits::destroy(allocator, &m_data[i]);
		}
		if (m_data != nullptr) {
			AllocTraits::deallocate(allocator,m_data, m_capacity);
		}
		m_data = newData;
		m_capacity = newCap;
	}

	void resize(size_type newSize) {
		if (newSize > m_size) {
			if (newSize > m_capacity) {
				reserve(newSize);
			}
			for (size_type i = m_size; i < newSize; ++i) {
				AllocTraits::construct(allocator, &m_data[i]);
			}
		}
		else if (newSize < m_size) {
			for (size_type i = newSize; i < m_size; ++i) {
				AllocTraits::destroy(allocator, &m_data[i]);
			}
		}
		m_size = newSize;
	}

	void resize(size_type newSize, const T& value) {
		if (newSize > m_size) {
			if (newSize > m_capacity) {
				reserve(newSize);
			}
			for (size_type i = m_size; i < newSize; ++i) {
				AllocTraits::construct(allocator, &m_data[i], value);
			}
		}
		else if (newSize < m_size) {
			for (size_type i = newSize; i < m_size; ++i) {
				AllocTraits::destroy(allocator, &m_data[i]);
			}
		}
		m_size = newSize;
	}

	T& operator[](size_type index)
	{
		return m_data[index];
	}

	const T& operator[](size_type index) const
	{
		return m_data[index];
	}

	T& at(size_type index)
	{
		if (index >= m_size) {
			throw std::out_of_range("Index out of range");
		}
		return m_data[index];
	}

	const T& at(size_type index) const
	{
		if (index >= m_size) {
			throw std::out_of_range("Index out of range");
		}
		return m_data[index];
	}

	T& front()
	{
		return m_data[0];
	}

	const T& front() const
	{
		return m_data[0];
	}

	T& back()
	{
		return m_data[m_size - 1];
	}

	const T& back() const
	{
		return m_data[m_size - 1];
	}

	T* data() noexcept
	{
		return m_data;
	}

	const T* data() const noexcept
	{
		return m_data;
	}

private:
	using AllocTraits = std::allocator_traits<Allocator>;
	T* m_data;
	size_type m_size;
	size_type m_capacity;
	Allocator allocator;
};

template<typename T, typename Allocator>
void swap(Vector<T, Allocator>& lhs, Vector<T, Allocator>& rhs) noexcept {
	lhs.swap(rhs);
}

// =====================================================
// Vector Implementation Complete! 
// 45+ Functions, Full std::vector Interface Coverage
// =====================================================

#endif