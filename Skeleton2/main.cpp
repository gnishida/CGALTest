/**
 * straight skeletonを使って、複雑な（特に凹型）ブロックを区画に分割する。
 *
 * @author Gen Nishida
 * @date 12/27/2014
 * @version 1.0
 */

#include <boost/shared_ptr.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/create_straight_skeleton_2.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K ;
typedef K::Point_2 KPoint;
typedef CGAL::Polygon_2<K> Polygon_2 ;
typedef CGAL::Straight_skeleton_2<K> Ss ;
typedef boost::shared_ptr<Ss> SsPtr ;

int main() {
	Polygon_2 poly;
	poly.push_back( KPoint(0, 0) );
	poly.push_back( KPoint(100, 50) );
	poly.push_back( KPoint(200, 0) );
	poly.push_back( KPoint(200, 130) );
	poly.push_back( KPoint(0, 130) );

	// You can pass the polygon via an iterator pair
	SsPtr iss = CGAL::create_interior_straight_skeleton_2(poly);

	cv::Mat m(130, 200, CV_8UC3, cv::Scalar(255, 255, 255));

	for (auto face = iss->faces_begin(); face != iss->faces_end(); ++face) {
		printf("Face %d:\n", face->id());
		auto edge0 = face->halfedge();
		auto edge = edge0;
		do {
			auto head = edge->vertex();
			auto tail = edge->opposite()->vertex();
			cv::line(m, cv::Point(tail->point().x(), tail->point().y()), cv::Point(head->point().x(), head->point().y()), cv::Scalar(255, 0, 0), 1);
			printf("(%.1lf, %.1lf) -> (%.1lf, %.1lf) bisector? %d\n", tail->point().x(), tail->point().y(), head->point().x(), head->point().y(), edge->is_bisector());
		} while ((edge = edge->next()) != edge0);
		printf("\n");
	}

	cv::imwrite("result.png", m);

	return 0;
}