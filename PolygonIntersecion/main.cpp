/**
 * Polygon intersecion
 *
 * 新規: version 1.0 (12/27/2014)
 * ２つのポリゴンのintersectionを計算する。
 *
 * 修正: version 1.1 (12/28/2014)
 * float型の座標を扱う場合は、Exact を使わないと問題があるみたい。
 *
 * 未解決問題: version 1.2 (12/28/2014)
 * P.is_clockwise_oriented()関数は、P.is_simple()を満たさないポリゴンではエラーが発生する。
 * おそらく、頂点間の距離が短すぎると、P.is_simple()を満たさないようだ。
 * 対策方法は不明。
 *
 * @author Gen Nishida
 * @date 12/28/2014
 * @version 1.2
 */

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <list>
#include <opencv/cv.h>
#include <opencv/highgui.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
//typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point_2;
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2> Pwh_list_2;

#define max(x, y) ((x >= y) ? x : y)
#define min(x, y) ((x <= y) ? x : y)

#define CGAL_POLYGON_NO_PRECONDITIONS

int main () {
	// Construct the two input polygons.
	Polygon_2 P;
	P.push_back(Point_2(968.5, 1564.8));
	P.push_back(Point_2(1067.2, 1336.1));
	P.push_back(Point_2(976.0, 1297.0));
	//P.push_back(Point_2(878.1, 1525.2)); //<-- この頂点のせいで、P.is_simple()を満たさない
	P.push_back(Point_2(877.8, 1524.8));
	P.push_back(Point_2(880.8, 1526.8));
	P.push_back(Point_2(921.0, 1544.4));

	if (!P.is_simple()) {
		printf("The polygon is self-intersecting.\n");
		return 0;
	}

	if (P.is_clockwise_oriented()) {
		P.reverse_orientation();
	}

	Polygon_2 Q1;
	Q1.push_back(Point_2(0, 0));
	Q1.push_back(Point_2(1067, 1250));
	Q1.push_back(Point_2(860, 1600));
	Q1.push_back(Point_2(0, 1300));
	if (!Q1.is_counterclockwise_oriented()) {
		Q1.reverse_orientation();
	}

	int max_x = -99999;
	int max_y = -99999;
	int min_x = 99999;
	int min_y = 99999;
	for (int i = 0; i < P.size(); ++i) {
		max_x = max(CGAL::to_double(P[i].x()), max_x);
		max_y = max(CGAL::to_double(P[i].y()), max_y);
		min_x = min(CGAL::to_double(P[i].x()), min_x);
		min_y = min(CGAL::to_double(P[i].y()), min_y);
	}
	/*
	for (int i = 0; i < Q.size(); ++i) {
		max_x = max(Q[i].x(), max_x);
		max_y = max(Q[i].y(), max_y);
		min_x = min(Q[i].x(), min_x);
		min_y = min(Q[i].y(), min_y);
	}
	*/

	cv::Mat m(max_y - min_y + 1, max_x - min_x + 1, CV_8UC3, cv::Scalar(255, 255, 255));
	for (int i = 0; i < P.size(); ++i) {
		Point_2 p1 = P[i];
		Point_2 p2 = P[(i + 1) % P.size()];

		cv::line(m, cv::Point(CGAL::to_double(p1.x()) - min_x, CGAL::to_double(p1.y()) - min_y), cv::Point(CGAL::to_double(p2.x()) - min_x, CGAL::to_double(p2.y()) - min_y), cv::Scalar(0, 0, 0), 1);
		cv::circle(m, cv::Point(CGAL::to_double(p1.x()) - min_x, CGAL::to_double(p1.y()) - min_y), 4, cv::Scalar(255, 0, 0), 1);
	}
	for (int i = 0; i < Q1.size(); ++i) {
		Point_2 p1 = Q1[i];
		Point_2 p2 = Q1[(i + 1) % Q1.size()];

		cv::line(m, cv::Point(CGAL::to_double(p1.x()) - min_x, CGAL::to_double(p1.y()) - min_y), cv::Point(CGAL::to_double(p2.x()) - min_x, CGAL::to_double(p2.y()) - min_y), cv::Scalar(255, 0, 0), 1);
	}

	// Compute the intersection of P and Q.
	Pwh_list_2 intR;
	Pwh_list_2::const_iterator it;
	CGAL::intersection (P, Q1, std::back_inserter(intR));
	
	for (it = intR.begin(); it != intR.end(); ++it) {
		if (!it->is_unbounded()) {
			for (auto edge = it->outer_boundary().edges_begin(); edge != it->outer_boundary().edges_end(); ++edge) {
				auto source = edge->source();
				auto target = edge->target();

				cv::line(m, cv::Point(CGAL::to_double(source.x()) - min_x, CGAL::to_double(source.y()) - min_y), cv::Point(CGAL::to_double(target.x()) - min_x, CGAL::to_double(target.y()) - min_y), cv::Scalar(255, 0, 0), 4);
			}
		}
	}

	cv::flip(m, m, 0);
	cv::imwrite("result.png", m);

	return 0;
}
