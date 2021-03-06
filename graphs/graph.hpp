#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "graph_gen.hpp"

#include <cstdlib>
#include <vector>

namespace graphs {

/*!
 * \brief The maximum number of edges that each vertex may have.
 */
constexpr unsigned max_edges_per_vertex = 4;

struct Vertex {
    unsigned id;

    VertexGen gen_info{};
};

bool operator==(Vertex const& lhs, Vertex const& rhs);

struct Edge {
    unsigned id;

    // The vertices the edge attaches
    unsigned v1;
    unsigned v2;
};

class MapGraph {
public:
    using VertexItr = std::vector<Vertex>::iterator;
    using CVertexItr = std::vector<Vertex>::const_iterator;
    using EdgeItr = std::vector<Edge>::iterator;
    using CEdgeItr = std::vector<Edge>::const_iterator;

private:
    std::vector<Vertex> vertices_;
    std::vector<Edge> edges_;

    GraphGen gen_info_;

    VertexItr addVertex();
    EdgeItr addEdge(VertexItr v1, VertexItr v2);

    /*!
     * \brief Gets a random vertex from the graph complete graph creates it if it doesn't exist.
     *
     * \return An iterator to the random vertex.
     */
    VertexItr randomNewVertex(VertexItr const& v);
    
public:
    MapGraph();

    VertexItr vertexFromId(unsigned vertex_id);
    CVertexItr vertexFromId(unsigned vertex_id) const;
    EdgeItr edgeFromId(unsigned edge_id);
    CEdgeItr edgeFromId(unsigned edge_id) const;

    VertexItr vertexBegin();
    CVertexItr cVertexBegin() const;
    VertexItr vertexEnd();
    CVertexItr cVertexEnd() const;

    EdgeItr edgeBegin();
    CEdgeItr cEdgeBegin() const;
    EdgeItr edgeEnd();
    CEdgeItr cEdgeEnd() const;

    MapGraph& process(unsigned id);
    MapGraph& connect_edge(unsigned vertex_id, unsigned edge_id);
};

}

#endif // GRAPH_HPP
