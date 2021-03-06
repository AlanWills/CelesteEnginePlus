#pragma once

#include "Memory/Allocators/PoolAllocator.h"
#include "Templates/ChooseClass.h"

#include <list>
#include <type_traits>
#include <iterator>


namespace Celeste
{
  template <typename T>
  class ResizeableAllocatorIterator
  {
    private:
      using Iterator = typename choose_class<
        std::is_const<T>::value,
        typename std::list<std::unique_ptr<PoolAllocator<typename std::remove_const<T>::type>>>::const_iterator,
        typename std::list<std::unique_ptr<PoolAllocator<T>>>::iterator>::type;
      
      using AllocatorType = typename choose_class<
        std::is_const<T>::value,
        typename const PoolAllocator<typename std::remove_const<T>::type>*,
        typename PoolAllocator<typename std::remove_const<T>::type>*>::type;

    public:
      using iterator_category = std::forward_iterator_tag;
      using value_type = T;
      using difference_type = ptrdiff_t;
      using pointer = T*;
      using reference = T&;

      ResizeableAllocatorIterator(typename Iterator allocatorEndIt, PoolAllocatorIterator<T> entityEndIt);
      ResizeableAllocatorIterator(typename Iterator allocatorIt, typename Iterator allocatorEndIt);
      ResizeableAllocatorIterator(const ResizeableAllocatorIterator<T>&) = default;
      ResizeableAllocatorIterator(ResizeableAllocatorIterator<T>&&) = default;
      
      ResizeableAllocatorIterator<T>& operator=(const ResizeableAllocatorIterator<T>&) = default;
      ResizeableAllocatorIterator<T>& operator=(ResizeableAllocatorIterator<T>&&) = default;

      ResizeableAllocatorIterator& operator++();
      ResizeableAllocatorIterator operator++(int);

      inline bool operator==(const ResizeableAllocatorIterator<T>& other) const { return m_entityIt.get() == other.get(); }
      inline bool operator!=(const ResizeableAllocatorIterator<T>& other) const { return !(*this == other); }

      inline reference operator*() const { return *m_entityIt; }
      inline pointer get() const { return m_entityIt.get(); }

    private:
      inline void next()
      {
        ++m_entityIt;
      }

      inline void advance()
      {
        while (m_entityIt == m_entityEndIt && m_allocatorDistance > 1)
        {
          // We are at the end of the allocator, but not in an allocated entry
          // We move to the next allocator
          --m_allocatorDistance;
          ++m_allocatorIt;
          m_entityIt = static_cast<AllocatorType>((*m_allocatorIt).get())->begin();
          m_entityEndIt = static_cast<AllocatorType>((*m_allocatorIt).get())->end();
        }
      }

      typename Iterator m_allocatorIt;
      size_t m_allocatorDistance;
      PoolAllocatorIterator<T> m_entityIt;
      PoolAllocatorIterator<T> m_entityEndIt;
  };

  //------------------------------------------------------------------------------------------------
  template <typename T>
  ResizeableAllocatorIterator<T>::ResizeableAllocatorIterator(
    typename Iterator allocatorIt,
    typename Iterator allocatorEndIt)
    : m_allocatorIt(allocatorIt),
      m_allocatorDistance(std::distance(allocatorIt, allocatorEndIt)),
      m_entityIt(static_cast<AllocatorType>((*m_allocatorIt).get())->begin()),
      m_entityEndIt(static_cast<AllocatorType>((*m_allocatorIt).get())->end())
  {
    advance();
  }

  //------------------------------------------------------------------------------------------------
  template <typename T>
  ResizeableAllocatorIterator<T>::ResizeableAllocatorIterator(
    typename Iterator allocatorEndIt, 
    PoolAllocatorIterator<T> entityEndIt)
    : m_allocatorIt(allocatorEndIt),
      m_allocatorDistance(0),
      m_entityIt(entityEndIt),
      m_entityEndIt(entityEndIt)
  {
  }

  //------------------------------------------------------------------------------------------------
  template <typename T>
  ResizeableAllocatorIterator<T>& ResizeableAllocatorIterator<T>::operator++()
  {
    next();
    advance();
    return *this;
  }

  //------------------------------------------------------------------------------------------------
  template <typename T>
  ResizeableAllocatorIterator<T> ResizeableAllocatorIterator<T>::operator++(int)
  {
    next();
    advance();
    return ResizeableAllocatorIterator<T>(*this);
  }
}