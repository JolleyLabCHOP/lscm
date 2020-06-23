#include "Vertex.h"
#include "HalfEdge.h"

using namespace MeshLib;

HalfEdge *Vertex::most_ccw_in_halfedge() {
	if (!m_boundary) { return m_halfedge; } //current half edge is the most ccw in halfedge 

	HalfEdge * he = m_halfedge->ccw_rotate_about_target();

	while (he != NULL) {
		m_halfedge = he;
		he = m_halfedge->ccw_rotate_about_target();
	}

	return m_halfedge;
}

HalfEdge *Vertex::most_clw_in_halfedge() {
	if (!m_boundary){ return most_ccw_in_halfedge()->ccw_rotate_about_target(); }

	HalfEdge * he = m_halfedge->clw_rotate_about_target();

	while (he != NULL) {
		m_halfedge = he;
		he = m_halfedge->clw_rotate_about_target();
	}

	return m_halfedge;
}

HalfEdge *Vertex::most_ccw_out_halfedge() {
	if (!m_boundary) { return most_ccw_in_halfedge()->he_sym(); }

	HalfEdge * he = m_halfedge->he_next();
	HalfEdge * ne = he->ccw_rotate_about_source();

	while (ne != NULL) {
		he = ne;
		ne = he->ccw_rotate_about_source();
	}

	return he;
}

HalfEdge *Vertex::most_clw_out_halfedge() {
	if (!m_boundary) { return most_ccw_out_halfedge()->ccw_rotate_about_source(); }

	HalfEdge * he = m_halfedge->he_next();
	HalfEdge * ne = he->clw_rotate_about_source();

	while (ne != NULL) {
		he = ne;
		ne = he->clw_rotate_about_source();
	}

	return he;
}

double MeshLib::Vertex::distance_to_point(Point &point)
{
	return sqrt(
		(m_point.x() - point.x()) * (m_point.x() - point.x()) +
		(m_point.y() - point.y()) * (m_point.y() - point.y()) +
		(m_point.z() - point.z()) * (m_point.z() - point.z()) );
}

bool FixedVertexDefinition::matches_vertex_point(Vertex* vertex) {
	if (!vertex) {
		return false;
	}

	double epsilon = 0.00001; // OBJ precision is 6
	return fabs(m_vertex_point.x() - vertex->point().x()) < epsilon
		  && fabs(m_vertex_point.y() - vertex->point().y()) < epsilon
		  && fabs(m_vertex_point.z() - vertex->point().z()) < epsilon;
}
