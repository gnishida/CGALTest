/**
 * ポリゴンの内側へのoffsetを計算する。
 * 道路網からブロックの生成する時などに使える。
 *
 * @author Gen Nishida
 * @date 12/26/2014
 * @version 1.0
 */

#include<vector>
#include<boost/shared_ptr.hpp>
#include<CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include<CGAL/Polygon_2.h>
#include<CGAL/create_offset_polygons_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef boost::shared_ptr<Polygon_2> PolygonPtr;
typedef std::vector<PolygonPtr> PolygonPtrVector;

int main() {
	Polygon_2 poly ;
	poly.push_back( Point(-1,-1) ) ;
	poly.push_back( Point(0,-12) ) ;
	poly.push_back( Point(1,-1) ) ;
	poly.push_back( Point(12,0) ) ;
	poly.push_back( Point(1,1) ) ;
	poly.push_back( Point(0,12) ) ;
	poly.push_back( Point(-1,1) ) ;
	poly.push_back( Point(-12,0) ) ;

	PolygonPtrVector inner_offset_polygons = CGAL::create_interior_skeleton_and_offset_polygons_2(0.1f, poly);

	for (int i = 0; i < inner_offset_polygons.size(); ++i) {
		printf("offset polygon %d\n", i + 1);
		for (auto v = inner_offset_polygons[i]->vertices_begin(); v != inner_offset_polygons[i]->vertices_end(); ++v) {
			printf("%lf, %lf\n", v->x(), v->y());
		}
	}

	return 0;
}
