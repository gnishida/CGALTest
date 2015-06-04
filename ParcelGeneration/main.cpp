/**
 * Polygon intersecion
 *
 * @author Gen Nishida
 * @date 12/27/2014
 * @version 1.0
 */

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <list>
#include <opencv/cv.h>
#include <opencv/highgui.h>

//typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point_2;
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2> Pwh_list_2;

int main () {
	// Construct the two input polygons.
	Polygon_2 P;
	P.push_back(Point_2(0, 0));
	P.push_back(Point_2(50, 0));
	P.push_back(Point_2(35, 15));
	P.push_back(Point_2(25, 5));
	P.push_back(Point_2(15, 15));

	Polygon_2 Q;
	Q.push_back(Point_2(0, 20));
	Q.push_back(Point_2(15, 5));
	Q.push_back(Point_2(25, 15));
	Q.push_back(Point_2(35, 5));
	Q.push_back(Point_2(50, 20));

	cv::Mat m(21, 51, CV_8UC3, cv::Scalar(255, 255, 255));

	// draw the original shapes
	for (auto edge = P.edges_begin(); edge != P.edges_end(); ++edge) {
		auto source = edge->source();
		auto target = edge->target();
		cv::line(m, cv::Point(source.x(), source.y()), cv::Point(target.x(), target.y()), cv::Scalar(0, 0, 0), 1);
	}
	for (auto edge = Q.edges_begin(); edge != Q.edges_end(); ++edge) {
		auto source = edge->source();
		auto target = edge->target();
		cv::line(m, cv::Point(source.x(), source.y()), cv::Point(target.x(), target.y()), cv::Scalar(0, 0, 0), 1);
	}


	// Compute the intersection of P and Q.
	Pwh_list_2 intR;
	Pwh_list_2::const_iterator it;
	CGAL::intersection (P, Q, std::back_inserter(intR));
	
	for (it = intR.begin(); it != intR.end(); ++it) {
		if (!it->is_unbounded()) {
			for (auto edge = it->outer_boundary().edges_begin(); edge != it->outer_boundary().edges_end(); ++edge) {
				auto source = edge->source();
				auto target = edge->target();

				cv::line(m, cv::Point(source.x(), source.y()), cv::Point(target.x(), target.y()), cv::Scalar(255, 0, 0), 1);
				//cv::line(m, cv::Point(CGAL::to_double(source.x()), CGAL::to_double(source.y())), cv::Point(CGAL::to_double(target.x()), CGAL::to_double(target.y())), cv::Scalar(255, 0, 0), 1);
			}
		}
	}

	cv::flip(m, m, 0);
	cv::imwrite("result.png", m);

	return 0;
}
