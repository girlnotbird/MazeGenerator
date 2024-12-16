#include "2d-node.h"

Node_2D::Node_2D() {};

/* LEGACY FUNCTION - Node_2D::validateConnection */
// void Node_2D::validateConnection(conn_dir direction, Connection *conn)
// {
//   if (this != conn->head && this != conn->tail)
//   {
//     throw CustomException("Connection must have this node at one terminal.");
//   }

//   Connection *requestedConnection{this->connections[(int)direction]};

//   Node_2D *complementaryNode{conn->head == this ? conn->tail : conn->head};
//   conn_dir complementaryDirection{(conn_dir)(((int)direction + 2) % 4)};
//   Connection *complementaryConnection{complementaryNode->connections[(int)complementaryDirection]};
  
//   if (requestedConnection != nullptr && requestedConnection != conn)
//   {
//     throw CustomException("Source node already connected on given edge.");
//   }
//   else if (complementaryConnection != nullptr && complementaryConnection != conn)
//   {
//     throw CustomException("Destination node already connected elsewhere on given edge.");
//   }
//   else if (contains(this->connections, conn) && requestedConnection != conn)
//   {
//     throw CustomException("Source node already has this connection on another edge.");
//   }
//   else if (contains(complementaryNode->connections, conn) && complementaryConnection != conn)
//   {
//     throw CustomException("Destination node already has this connection on another edge.");
//   }

//   return;
// };

/* LEGACY FUNCTION - Node_2D::getConnection */
// Connection *Node_2D::getConnection(conn_dir direction)
// {
//   return this->connections[(int)direction];
// };

/* LEGACY FUNCTION - Node_2D::setConnection */
// void Node_2D::setConnection(conn_dir direction, Connection *conn)
// {
//   try
//   {
//     this->validateConnection(direction, conn);
//     this->connections[(int)direction] = conn;
//   }
//   catch (CustomException ex)
//   {
//     std::cerr << "Connection exception thrown: " << ex.what() << "\n";
//   };
// };

/* LEGACY FUNCTION - Node_2D::openConnection */
// void Node_2D::openConnection(conn_dir dir)
// {
//   this->connections.at((int)dir)->status = conn_stat::OPEN;
// };

/* LEGACY FUNCTION - Node_2D::closeConnection */
// void Node_2D::closeConnection(conn_dir dir)
// {
//   this->connections.at((int)dir)->status = conn_stat::CLOSED;
// };

/* LEGACY FUNCTION - createConnection */
// Connection* createConnection()
// { 
//   return new Connection {
//       conn_stat::CLOSED,
//       nullptr,
//       nullptr
//     };
// };