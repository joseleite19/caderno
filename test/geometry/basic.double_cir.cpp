// Geogebra: https://ggbm.at/GT8w9nCV
#include <bits/stdc++.h>
#define cout if (1) cout

using namespace std;
typedef long long int ll;
typedef pair<ll,ll> pii;
#define pb push_back

typedef double cood;
cood eps = 1e-8;

#include "gtest/gtest.h"
#include "../../code/geometry/basic.cpp"

ostream& operator<<(ostream& os, vec o) { return os << '(' << o.x << ", " << o.y << ')'; }
ostream& operator<<(ostream& os, lin o) { return os << '[' << o.p << "*(x,y) = " << o.c << ']'; }
ostream& operator<<(ostream& os, cir o) { return os << '[' << o.c << o.r << ']'; }

bool isClose (cood a, cood b) {
	if (abs(a - b) <= eps) return true;
	return min(a*(1-eps),a*(1+eps)) <= b + 1e-15 && b <= max(a*(1-eps),a*(1+eps)) + 1e-15;
}
bool isClose (vec a, vec b) { return isClose(a.x,b.x) && isClose(a.y,b.y); }

bool operator != (vec a, vec b)
{ return !(a==b); }
bool operator == (cir a, cir b)
{ return a.c == b.c && abs(a.r-b.r) <= eps; }

vec A[] = {vec(1,4), vec(3,2), vec(7,6), vec(4,6)},
	B = vec(4,14),
	E[] = {vec(72,92), vec(78,90), vec(84,88), vec(54,98)},
	F[] = {vec(-30,10), vec(-35,10), vec(-22,5), vec(-23,7), vec(-28,9), vec(-27,11), vec(-32,13), vec(-34,5), vec(-34,7), vec(-35,8), vec(-22,15), vec(-27,17)},
	G[] = {vec(-10,-9), vec(-2,-9), vec(3,-9), vec(8,-9), vec(-4,1), vec(2,-4), vec(14,1), vec(-5,-21), vec(-21,-25), vec(7,-3), vec(-15,-36), vec(-17,-5), vec(-4,3), vec(3,-14), vec(-22,-16), vec(7,-3)};

TEST(geometry_basic_cir, cir) {
	cir a0(A[0],A[1],A[2]);

	EXPECT_EQ(a0, cir(vec(4,5), sqrt(10))) << "Creates correct circle";

	vec v[] = {A[0],A[1],A[2]};
	sort(v, v+3);
	do {
		EXPECT_EQ(cir(v[0],v[1],v[2]), a0) << "Doesn't depend on order";
	} while (next_permutation(v,v+3));

	vec D(4,6);
	cir a1(vec(3.1,4.1), sqrt(4.42));
	EXPECT_EQ(cir(A[0],A[1],A[3]), a1) << "Creates a different circle";
}

TEST(geometry_basic_cir, arc_area) {
	cir b(B,4);
	vec ray(4,0);

	vec Ba[] = {b.c + ray*.5, b.c + ray, b.c + ray*1.5};
	ray = ray.rot90();	
	vec Bb[] = {b.c + ray*.5, b.c + ray, b.c + ray*1.5};
	ray = ray.rotate(3*pi/4);
	vec Bc[] = {b.c + ray*.5, b.c + ray, b.c + ray*1.5};

	EXPECT_DOUBLE_EQ(b.arc_area(Ba[1],Bb[1]),4*pi) << "Correct";
	EXPECT_DOUBLE_EQ(b.arc_area(Ba[1],Bc[1]),-6*pi) << "Oriented";

	for (vec * u : {Ba,Bb,Bc}) {
		for (vec * v : {Ba,Bb,Bc}) {
			if (u == v) continue;
			double res = b.arc_area(u[0],v[0]);
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					EXPECT_DOUBLE_EQ(res,b.arc_area(u[i],v[j])) << "Doesn't depend on ray length i = " << i << " j = " << j;
		}

		for (int i = 0; i < 3; i++) {
			EXPECT_NEAR(0.,b.arc_area(b.c,u[i]),1e-12) << "Degenerate with center i = " << i;
			EXPECT_NEAR(0.,b.arc_area(u[i],b.c),1e-12) << "Degenerate with center i = " << i;
			for (int j = 0; j < 3; j++) {
				EXPECT_NEAR(0.,b.arc_area(u[i],u[j]),1e-12) << "Degenerate regular i = " << i << " j = " << j;
				EXPECT_NEAR(pi*b.r*b.r/2,abs(b.arc_area(u[i],b.c - (u[j] - b.c))),1e-12) << "Degenerate opposite i = " << i << " j = " << j;
				EXPECT_NEAR(pi*b.r*b.r/2,abs(b.arc_area(b.c - (u[j] - b.c),u[i])),1e-12) << "Degenerate opposite i = " << i << " j = " << j;
				EXPECT_NEAR(0.,b.arc_area(u[i],b.c + (u[j] - b.c)*1.5),1e-12) << "Degenerate further i = " << i << " j = " << j;
				EXPECT_NEAR(0.,b.arc_area(u[i],b.c + (u[j] - b.c)*.5),1e-12) << "Degenerate closer i = " << i << " j = " << j;
			}
		}
	}

	cir b_big(b.c,b.r*2);
	EXPECT_NEAR(b_big.arc_area(Ba[0],Bb[1])/b.arc_area(Ba[0],Bb[1]), 4,1e-12) << "Quadratic on ray";
}

TEST(geometry_basic_cir, arc_len) {
	cir b(B,4);
	vec ray(4,0);

	vec Ba[] = {b.c + ray*.5, b.c + ray, b.c + ray*1.5};
	ray = ray.rot90();	
	vec Bb[] = {b.c + ray*.5, b.c + ray, b.c + ray*1.5};
	ray = ray.rotate(3*pi/4);
	vec Bc[] = {b.c + ray*.5, b.c + ray, b.c + ray*1.5};

	EXPECT_DOUBLE_EQ(b.arc_len(Ba[1],Bb[1]),2*pi) << "Correct";
	EXPECT_DOUBLE_EQ(b.arc_len(Ba[1],Bc[1]),-3*pi) << "Oriented";

	for (vec * u : {Ba,Bb,Bc}) {
		for (vec * v : {Ba,Bb,Bc}) {
			if (u == v) continue;
			double res = b.arc_len(u[0],v[0]);
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					EXPECT_DOUBLE_EQ(res,b.arc_len(u[i],v[j])) << "Doesn't depend on ray length i = " << i << " j = " << j;
		}

		for (int i = 0; i < 3; i++) {
			EXPECT_NEAR(0.,b.arc_len(b.c,u[i]),1e-12) << "Degenerate with center i = " << i;
			EXPECT_NEAR(0.,b.arc_len(u[i],b.c),1e-12) << "Degenerate with center i = " << i;
			for (int j = 0; j < 3; j++) {
				EXPECT_NEAR(0.,b.arc_len(u[i],u[j]),1e-12) << "Degenerate regular i = " << i << " j = " << j;
				EXPECT_NEAR(pi*b.r,abs(b.arc_len(u[i],b.c - (u[j] - b.c))),1e-12) << "Degenerate opposite i = " << i << " j = " << j;
				EXPECT_NEAR(pi*b.r,abs(b.arc_len(b.c - (u[j] - b.c),u[i])),1e-12) << "Degenerate opposite i = " << i << " j = " << j;
				EXPECT_NEAR(0.,b.arc_len(u[i],b.c + (u[j] - b.c)*1.5),1e-12) << "Degenerate further i = " << i << " j = " << j;
				EXPECT_NEAR(0.,b.arc_len(u[i],b.c + (u[j] - b.c)*.5),1e-12) << "Degenerate closer i = " << i << " j = " << j;
			}
		}
	}
	cir b_big(b.c,b.r*2);
	EXPECT_NEAR(b_big.arc_len(Ba[0],Bb[1])/b.arc_len(Ba[0],Bb[1]), 2,1e-12) << "Linear on ray";
}

::testing::AssertionResult is_on_circle (vec v, cir c) { 
	if (c.border(v))
		return ::testing::AssertionSuccess();
	return ::testing::AssertionFailure() << v << " is not on " << c << ", has distance " << v.nr(c.c);
}

::testing::AssertionResult check_cir_border_inter (cir a, cir b, bool once) {
	pair<vec,vec> res = a.border_inter(b);
	::testing::AssertionResult ret = ::testing::AssertionSuccess();
	if (once) {
		if (!isClose(res.first,res.second)) 
			return ret = ::testing::AssertionFailure() << "two different intersections: " << res.first << " and " << res.second;
	} else {
		if (isClose(res.first,res.second))
			return ret = ::testing::AssertionFailure() << "only one intersection: " << res.first;
	}
	
	if (ret) ret = is_on_circle(res.first, a);
	if (ret) ret = is_on_circle(res.first, b);
	if (ret) ret = is_on_circle(res.second, a);
	if (ret) ret = is_on_circle(res.second, b);

	return ret;
}

TEST(geometry_basic_cir, border_inter) {
	double e_d = E[0].nr(E[1]);
	cir e[] = {
		cir(E[0],1.5*e_d), // green
		cir(E[1],.5*e_d), cir(E[1],e_d), cir(E[1],1.8*e_d), // orange
		cir(E[2],e_d), cir(E[2],2*e_d), cir(E[2],2.3*e_d), cir(E[2],3*e_d), // purple
		cir(E[3],2*e_d), cir(E[3],1.5*e_d) // blue
	};

	for (int i = 1; i <= 9; i++) {
		EXPECT_TRUE(check_cir_border_inter(e[0], e[i], (i == 1 || i == 9))) << " where i = " << i;
		EXPECT_TRUE(check_cir_border_inter(e[i], e[0], (i == 1 || i == 9))) << " where i = " << i;
	}

	EXPECT_TRUE(check_cir_border_inter(e[3],e[4],false)) << "Inters after center";
	EXPECT_TRUE(check_cir_border_inter(e[3],e[6],false)) << "Inters after center";
	EXPECT_TRUE(check_cir_border_inter(e[4],e[3],false)) << "Inters after center";
	EXPECT_TRUE(check_cir_border_inter(e[6],e[3],false)) << "Inters after center";
	EXPECT_TRUE(check_cir_border_inter(e[8],e[7],true)) << "Borders touch";

	EXPECT_ANY_THROW(e[1].border_inter(e[9])) << "No intersection";
	EXPECT_ANY_THROW(e[9].border_inter(e[1])) << "No intersection";
	EXPECT_ANY_THROW(e[4].border_inter(e[6])) << "Fully contained";
	EXPECT_ANY_THROW(e[6].border_inter(e[4])) << "Fully contained";
	EXPECT_ANY_THROW(e[2].border_inter(e[7])) << "Fully contained different centers";
	EXPECT_ANY_THROW(e[7].border_inter(e[2])) << "Fully contained different centers";

	for (int i = 0; i <= 9; i++) {
		for (int j = 0; j <= 0; j++) {
			if (e[i].border(E[j])) {
				EXPECT_TRUE(check_cir_border_inter(e[i], cir(E[j],0), true)) << "Degenerate with inter";
				EXPECT_TRUE(check_cir_border_inter(cir(E[j],0), e[i], true)) << "Degenerate with inter";
			} else {
				EXPECT_ANY_THROW(e[i].border_inter(cir(E[j],0))) << "Degenerate no inter";
				EXPECT_ANY_THROW(cir(E[j],0).border_inter(e[i])) << "Degenerate no inter";
			}
		}
	}
}

::testing::AssertionResult is_on_lin (vec v, vec a, vec b) { 
	return v.ccw(a,b) == 0 ? ::testing::AssertionSuccess() : ::testing::AssertionFailure() << v << " " << a << " and " << b << " not collinear";
}

::testing::AssertionResult check_cir_inter_lin (cir c, vec a, vec b, bool once) {
	std::pair<vec,vec> res = c.border_inter_lin(a,b);
	::testing::AssertionResult ret = ::testing::AssertionSuccess();
	if (once) {
		if (res.first != res.second) 
			return ret = ::testing::AssertionFailure() << "two different intersections: " << res.first << " and " << res.second;
	} else {
		if (res.first == res.second)
			return ret = ::testing::AssertionFailure() << "only one intersection: " << res.first;
	}
	
	if (ret) ret = is_on_circle(res.first, c);
	if (ret) ret = is_on_circle(res.second, c);
	if (ret) ret = is_on_lin(res.first, a, b);
	if (ret) ret = is_on_lin(res.second, a, b);

	if (res.second.sq(a) + eps < res.first.sq(a)) ret = ::testing::AssertionFailure() << "second (" << res.second << ") was closest to a (" << a << ") than first (" << res.first << ")";

	return ret;
}

TEST(geometry_basic_cir, border_inter_lin) {
	cir f(F[0],5);
	vec f_o(-38,5);
	
	for (int i = 1; i <= 11; i++) {
		EXPECT_TRUE(check_cir_inter_lin(f, f_o, F[i], (i == 2 || i == 7))) << " where i = " << i;
		EXPECT_TRUE(check_cir_inter_lin(f, F[i], f_o, (i == 2 || i == 7))) << " where i = " << i;
	}
	
	EXPECT_TRUE(check_cir_inter_lin(f, F[5], F[6], false)) << " both in";
	EXPECT_TRUE(check_cir_inter_lin(f, F[6], F[5], false)) << " both in";
	EXPECT_TRUE(check_cir_inter_lin(f, F[0], F[3], false)) << " with center";
	EXPECT_TRUE(check_cir_inter_lin(f, F[3], F[0], false)) << " with center";
	EXPECT_TRUE(check_cir_inter_lin(f, F[2], F[3], false)) << " close inters";
	EXPECT_TRUE(check_cir_inter_lin(f, F[3], F[2], false)) << " close inters";
	EXPECT_TRUE(check_cir_inter_lin(f, F[2], F[7], true)) << " tangent";
	EXPECT_TRUE(check_cir_inter_lin(f, F[7], F[2], true)) << " tangent";
	EXPECT_TRUE(check_cir_inter_lin(cir(F[5],20), F[10], F[11], false)) << " other circle";
	EXPECT_TRUE(check_cir_inter_lin(cir(F[5],20), F[11], F[10], false)) << " other circle";

	EXPECT_ANY_THROW(f.border_inter_lin(F[10],F[11])) << " no inter";
	EXPECT_ANY_THROW(f.border_inter_lin(F[11],F[10])) << " no inter";
	EXPECT_ANY_THROW(f.border_inter_lin(F[2],F[11])) << " almost has inter";
	EXPECT_ANY_THROW(f.border_inter_lin(F[11],F[2])) << " almost has inter";

	for (int i = 0; i <= 11; i++) {
		if (f.border(F[i])) {
			EXPECT_TRUE(check_cir_inter_lin(f, F[i], F[i], 1)) << " degenerate where i = " << i;
		} else {
			EXPECT_ANY_THROW(f.border_inter_lin(F[i],F[i])) << " no inter degenerate where i = " << i;
		}
		if (i) {
			EXPECT_TRUE(check_cir_inter_lin(f, F[i], F[0], 0)) << " degenerate with center where i = " << i;
			EXPECT_TRUE(check_cir_inter_lin(f, F[0], F[i], 0)) << " degenerate with center where i = " << i;
		}
	}
}

TEST(geometry_basic_cir, triang_inter) {
	cir g(G[0],13);

	for (int i : {1,2})
		for (int j : {4,5})
			EXPECT_DOUBLE_EQ(g.triang_inter(G[i],G[j]), g.c.cross(G[i],G[j])/2) << "full triangle where i = " << i << " j = " << j;

	for (int i : {2,3})
		for (int j : {5,6})
			if (i != 2 || j != 5) {
				EXPECT_DOUBLE_EQ(g.triang_inter(G[i],G[j]), g.arc_area(G[i],G[j])) << "full arc where i = " << i << " j = " << j;
			}

	EXPECT_DOUBLE_EQ(g.triang_inter(G[3],G[4]), g.arc_area(G[3],G[5]) + g.c.cross(G[5],G[4])/2) << "intersecting once";
	EXPECT_DOUBLE_EQ(g.triang_inter(G[9],G[10]), g.arc_area(G[9],G[2]) + g.c.cross(G[2],G[7])/2 + g.arc_area(G[7],G[10])) << "intersecting twice";

	for (int i = 1; i <= 14; i++) {
		for (int j = 1; j <= 14; j++) {
			if (i == j) continue;
			EXPECT_EQ(g.triang_inter(G[i],G[j]), -g.triang_inter(G[j],G[i])) << "consistent where i = " << i << " and j = " << j;
			if (g.c.ccw(G[i],G[j]) == 0) {
				EXPECT_DOUBLE_EQ(g.triang_inter(G[i],G[j]), 0) << "colinear where i = " << i << " and j = " << j;
			}

			if (g.contains(G[i]) && g.contains(G[j])) {
				EXPECT_DOUBLE_EQ(g.triang_inter(G[i],G[j]), g.c.cross(G[i],G[j])/2) << "full triangle where i = " << i << " j = " << j;
			} else {
				// XXX important test!
				// if this fails, the intersection point might be chosen wrong
				EXPECT_PRED_FORMAT2(::testing::DoubleLE, abs(g.triang_inter(G[i],G[j])), abs(g.c.cross(G[i],G[j])/2)) << "not full triangle where i = " << i << " j = " << j;
			}
		}

		EXPECT_NEAR(g.triang_inter(G[i],G[i]), 0, 1e-12) << "Degenerate same i = " << i;
		EXPECT_NEAR(g.triang_inter(G[i],G[i] + (G[i] - g.c)), 0, 1e-12) << "Degenerate colin i = " << i;
		EXPECT_NEAR(g.triang_inter(G[i],G[i] - (G[i] - g.c)/2), 0, 1e-12) << "Degenerate colin i = " << i;
		EXPECT_NEAR(g.triang_inter(G[0],G[i]), 0, 1e-12) << "Degenerate with center i = " << i;
		EXPECT_NEAR(g.triang_inter(G[i],G[0]), 0, 1e-12) << "Degenerate with center i = " << i;
	}
}

::testing::AssertionResult test_cir_tan (cir c, vec v) { 
	pair<vec,vec> res = c.tan(v);
	for (vec p : {res.first, res.second}) {
		::testing::AssertionResult r = is_on_circle(p,c);
		if (!r) return r << " Tangent point is not on border of circle";
		r  = check_cir_inter_lin(c, v, p, true);
		if (!r) return r << " Tangent line doesn't intersect exactly once";
	}
	return ::testing::AssertionSuccess();
}

TEST(geometry_basic_cir, tan) {
	cood oldeps = eps; eps = 1e-6; // less precision for this test

	cir g(G[0],13);

	EXPECT_TRUE(test_cir_tan(g,G[8])) << "standart " << G[8] << " and " << g;
	EXPECT_TRUE(test_cir_tan(g,G[14])) << "standart " << G[14] << " and " << g;
	EXPECT_TRUE(test_cir_tan(g,G[7])) << "border " << G[7] << " and " << g;
	EXPECT_ANY_THROW(g.tan(G[1])) << "inside " << G[1] << " and " << g;

	eps = oldeps;
}
