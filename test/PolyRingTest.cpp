#include <lattice/PolyRing.hpp>
#include <lattice/Helper.hpp>

#include <gtest/gtest.h>

#include <cstdlib>
#include <iostream>


TEST( PolyRingTest, parameters ) {
  size_t degree = 256;
  size_t k = 10;
  auto ring = lattice::PolyRing{degree, k};

  ASSERT_EQ( ring.degree(), degree );
  ASSERT_EQ( ring.k(), k );
  ASSERT_EQ( ring.modulus(), size_t(1) << k );
}

TEST( PolyRingTest, setCoeff ) {
  size_t degree = 256;
  size_t k = 8;
  auto ring = lattice::PolyRing{degree, k};

  for ( size_t i = 0; i < degree; ++i ) 
    ring.set( i, i );

  for ( size_t i = 0; i < degree; ++i )
    ASSERT_EQ( ring.get( i ), i );
}

TEST( PolyRingTest, modQ ) {
  size_t degree = 256;
  size_t k = 8;
  auto ring = lattice::PolyRing{degree, k};

  // (2^k + i) % Q should equal i
  for ( size_t i = 0; i < degree; ++i )
    ring.set( i, (size_t(1) << k) + i );

  for ( size_t i = 0; i < degree; ++i )
    ASSERT_EQ( i, i );
}

TEST( PolyRingTest, plusEquals ) {
  size_t degree = 256;
  size_t k = 8;
  size_t modulus = size_t(1) << k;
  auto ring = lattice::PolyRing{degree, k};
  auto ring2 = lattice::PolyRing{degree, k};
  ring.uniformInit();
  ring2.uniformInit();

  std::vector<size_t> expected;
  for ( size_t i = 0; i < degree; ++i )
    expected.push_back( (ring.get( i ) + ring2.get( i )) % modulus );

  ring += ring2;

  for ( size_t i = 0; i < degree; ++i )
    ASSERT_EQ( expected[i], ring.get( i ) );
}

TEST( PolyRingTest, plus ) {
  size_t degree = 256;
  size_t k = 8;
  size_t modulus = size_t(1) << k;
  auto left = lattice::PolyRing{degree, k};
  auto right = lattice::PolyRing{degree, k};
  auto result = lattice::PolyRing{degree, k};
  left.uniformInit();
  right.uniformInit();

  std::vector<size_t> expected;
  for ( size_t i = 0; i < degree; ++i )
    expected.push_back( (left.get( i ) + right.get( i )) % modulus );

  result = left + right;

  for ( size_t i = 0; i < degree; ++i )
    ASSERT_EQ( expected[i], result.get( i ) );
}

TEST( PolyRingTest, timesEquals ) {
  size_t degree = 3;
  size_t k = 4;
  auto ring = lattice::PolyRing{degree, k};
  auto ring2 = lattice::PolyRing{degree, k};

  ring.set( 0, 1 );
  ring.set( 1, 1 );
  ring.set( 2, 1 );

  ring2.set( 0, 1 );
  ring2.set( 1, 1 );
  ring2.set( 2, 1 );

  // (x^2 + x + 1)^2 % x^2 + 1
  ring *= ring;
  // result = 2x^2 + 15x + 1
  ASSERT_EQ( size_t(15), ring.get( 0 ) );
  ASSERT_EQ( size_t(1), ring.get( 1 ) );
  ASSERT_EQ( size_t(3), ring.get( 2 ) );
}

