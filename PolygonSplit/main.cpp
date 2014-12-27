/**
 * Polygonを分割する。
 *
 * @author Gen Nishida
 * @date 12/27/2014
 * @version 1.0
 */

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Partition_traits_2.h>
#include <CGAL/partition_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_polygon_2.h>
#include <cassert>
#include <list>
#include <opencv/cv.h>
#include <opencv/highgui.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Partition_traits_2<K> Traits;
typedef Traits::Point_2 Point_2;
typedef Traits::Polygon_2 Polygon_2;
typedef Polygon_2::Vertex_iterator Vertex_iterator;
typedef std::list<Polygon_2> Polygon_list;
typedef CGAL::Creator_uniform_2<int, Point_2> Creator;
typedef CGAL::Random_points_in_square_2<Point_2, Creator> Point_generator;

void make_polygon(Polygon_2& polygon) {
	polygon.push_back(Point_2(391, 374));
	polygon.push_back(Point_2(240, 431));
	polygon.push_back(Point_2(252, 340));
	polygon.push_back(Point_2(374, 320));
	polygon.push_back(Point_2(289, 214));
	polygon.push_back(Point_2(134, 390));
	polygon.push_back(Point_2( 68, 186));
	polygon.push_back(Point_2(154, 259));
	polygon.push_back(Point_2(161, 107));
	polygon.push_back(Point_2(435, 108));
	polygon.push_back(Point_2(208, 148));
	polygon.push_back(Point_2(295, 160));
	polygon.push_back(Point_2(421, 212));
	polygon.push_back(Point_2(441, 303));
}

int main() {
	Polygon_2 polygon;
	Polygon_list partition_polys;

	/*
	CGAL::random_polygon_2(50, std::back_inserter(polygon),
	Point_generator(100));
	*/

	make_polygon(polygon);

	CGAL::approx_convex_partition_2(polygon.vertices_begin(), polygon.vertices_end(), std::back_inserter(partition_polys));
	assert(CGAL::convex_partition_is_valid_2(polygon.vertices_begin(), polygon.vertices_end(), partition_polys.begin(),	partition_polys.end()));

	cv::Mat m(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));

	for (auto edge = polygon.edges_begin(); edge != polygon.edges_end(); ++edge) {
		auto source = edge->source();
		auto target = edge->target();
		cv::line(m, cv::Point(source.x(), source.y()), cv::Point(target.x(), target.y()), cv::Scalar(0, 0, 0), 1);
	}

	for (auto polygon = partition_polys.begin(); polygon != partition_polys.end(); ++polygon) {
		for (auto edge = polygon->edges_begin(); edge != polygon->edges_end(); ++edge) {
			auto head = edge->source();
			auto tail = edge->target();
			cv::line(m, cv::Point(head.x(), head.y()), cv::Point(tail.x(), tail.y()), cv::Scalar(255, 0, 0), 1);
		}
	}

	cv::imwrite("result.png", m);

	return 0;
}