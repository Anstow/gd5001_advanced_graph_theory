#include "graph.hpp"

#include <random>
#include <iterator>
#include <algorithm>

using namespace graphs;

bool operator==(Vertex const& lhs, Vertex const& rhs) {
    return lhs.id == rhs.id;
}

MapGraph::VertexItr MapGraph::addVertex() {
    vertices_.push_back(Vertex{gen_info_.next_id});
    gen_info_.next_id++;
    return vertices_.end() - 1;
}

MapGraph::EdgeItr MapGraph::addEdge(VertexItr v1, VertexItr v2) {
    edges_.push_back(Edge{gen_info_.next_id, v1->id, v2->id});
    v1->gen_info.no_edges++;
    v2->gen_info.no_edges++;
    gen_info_.next_id++;
    // Tell the display that new edges have been connected
    EdgeItr e = edges_.end() - 1;
    connect_edge(v1->id, e->id);
    connect_edge(v2->id, e->id);

    return e;
}

MapGraph::VertexItr MapGraph::randomNewVertex(VertexItr const& v) {
    // The probablitity that the new edge goes to an already generated vertex
    double prob_old_vertex = vertices_.size() / expected_no_vertices;
    if (std::generate_canonical<double, 10>(gen_info_.random_gen) < prob_old_vertex) {
        // Grab one of the old vertices
        auto v2 = std::begin(vertices_) + std::uniform_int_distribution<size_t>{0, vertices_.size() - 1}(gen_info_.random_gen);
        // Stop edges connecting twice to the same vertex
        if (v == v2) {
            return addVertex();
        } else {
            return v2;
        }
    } else {
        // Get a new vertex
        return addVertex();
    }
}

MapGraph::MapGraph() {
    // This assume there is more than one element in vertices
    addVertex();
    process(std::begin(vertices_)->id);
}

MapGraph::VertexItr MapGraph::vertexFromId(unsigned vertex_id) {
    return std::find_if(std::begin(vertices_), std::end(vertices_), 
            [vertex_id] (Vertex const& v) {
                return v.id == vertex_id; 
            });
}

MapGraph::CVertexItr MapGraph::vertexFromId(unsigned vertex_id) const {
    return std::find_if(vertices_.cbegin(), vertices_.cend(), 
            [vertex_id] (Vertex const& v) {
                return v.id == vertex_id;
            });
}

MapGraph::VertexItr MapGraph::vertexBegin() {
    return vertices_.begin();
}

MapGraph::VertexItr MapGraph::vertexEnd() {
    return vertices_.end();
}

MapGraph::CVertexItr MapGraph::cVertexBegin() const {
    return vertices_.cbegin();
}

MapGraph::CVertexItr MapGraph::cVertexEnd() const {
    return vertices_.cend();
}

MapGraph::EdgeItr MapGraph::edgeBegin() {
    return edges_.begin();
}

MapGraph::EdgeItr MapGraph::edgeEnd() {
    return edges_.end();
}

MapGraph::CEdgeItr MapGraph::cEdgeBegin() const
{
    return edges_.cbegin();
}

MapGraph::CEdgeItr MapGraph::cEdgeEnd() const
{
    return edges_.cend();
}

MapGraph::EdgeItr MapGraph::edgeFromId(unsigned edge_id) {
    return std::find_if(std::begin(edges_), std::end(edges_), 
            [edge_id] (Edge const& e) {
                return e.id == edge_id; 
            });
}

MapGraph::CEdgeItr MapGraph::edgeFromId(unsigned edge_id) const {
    return std::find_if(std::cbegin(edges_), std::cend(edges_), 
            [edge_id] (Edge const& e) {
                return e.id == edge_id; 
            });
}

MapGraph& MapGraph::process(unsigned id) {
    // This isn't a vertex
    auto v = vertexFromId(id);
    if (v == vertices_.end()) 
        return *this;
    size_t vertexOffset = v - vertices_.begin();
    if (v->gen_info.processed) {
        // TODO: Reprocess the vertex here so the connections map change
    } else {
        v->gen_info.processed = true;
        double prob_edge_exists =
            average_no_edges_per_vertex / max_edges_per_vertex;
        auto bdist = std::binomial_distribution<unsigned>(
                max_edges_per_vertex - v->gen_info.no_edges, prob_edge_exists);
        unsigned edges_to_add = bdist(gen_info_.random_gen);
        for (unsigned i = 0; i < edges_to_add; ++i) {
            // Note this invalidate the vertex v
            auto v2 = randomNewVertex(v);
            v = vertices_.begin() + vertexOffset;
            addEdge(v, v2);
        }
    }
    return *this;
}

MapGraph& MapGraph::connect_edge(unsigned, unsigned) {
    return *this;
}
