#include <stdafx.h>
#include "main.h"
#include "TradeGraph.h"
#include "GameWorld.h"

TradeGraph::TradeGraph(const unsigned char player,const GameWorldGame * const gwg)
: gwg(gwg), player(player), size(gwg->GetWidth() / TGN_SIZE,gwg->GetHeight() / TGN_SIZE)
{
	if(gwg->GetWidth() % TGN_SIZE > 0) ++size.x;
	if(gwg->GetHeight() % TGN_SIZE > 0) ++size.y;
	trade_graph.resize(size.x*size.y);
}

/// Creates a new complete graph
void TradeGraph::Create()
{
	for(MapCoord y=0;y<size.y;++y)
		for(MapCoord x=0;x<size.x;++x)
			FindMainPoint(Point<MapCoord>(x,y));

	for(MapCoord y=0;y<size.y;++y)
		for(MapCoord x=0;x<size.x;++x)
		{
			for(unsigned d = 0;d<8;++d)
				UpdateEdge(Point<MapCoord>(x,y),d,NULL);
		}
}

/// Creates the graph at the beginning of the game using the data of the graph of another player
void TradeGraph::CreateWithHelpOfAnotherPlayer(const TradeGraph& helper,const GameClientPlayerList& players)
{
	for(MapCoord y=0;y<size.y;++y) for(MapCoord x=0;x<size.x;++x)
	{
		Point<MapCoord> p(x,y);
		// Player hqs far away from this point?
		unsigned nearest_hq = oo;
		for(unsigned i = 0;i<players.getCount();++i)
		{
			unsigned new_distance = gwg->CalcDistance(helper.GetNode(p).main_pos,Point<MapCoord>(players[i].hqx,players[i].hqy));

			if(new_distance < nearest_hq)
				nearest_hq = new_distance;
		}

		if(nearest_hq >= TGN_SIZE*2)
			GetNode(p) = helper.GetNode(p);
		else
			FindMainPoint(p);
	}
	for(MapCoord y=0;y<size.y;++y) for(MapCoord x=0;x<size.x;++x)
	{
		Point<MapCoord> p(x,y);
		// Player hqs far away from this point?
		unsigned nearest_hq = oo;
		for(unsigned i = 0;i<players.getCount();++i)
		{
			unsigned new_distance = gwg->CalcDistance(helper.GetNode(p).main_pos,Point<MapCoord>(players[i].hqx,players[i].hqy));

			if(new_distance < nearest_hq)
				nearest_hq = new_distance;
		}

		if(nearest_hq < TGN_SIZE*2)
			for(unsigned d = 0;d<8;++d)
				UpdateEdge(Point<MapCoord>(x,y),d,&helper);
	}
}


/// Gets the next direction the caravane has to take
unsigned char TradeRoute::GetNextDir()
{
	// Test the route in the trade graph
	for(unsigned i = global_pos;i<global_route.size();++i)
	{
		unsigned char next_dir = global_route[i];
		Point<MapCoord> pos = TradeGraphNode::ConverToTGCoords(current_pos);
		// Next direction not possible?
		if(!tg->GetNode(pos).dirs[next_dir])
			return RecalcGlobalRoute();
	}

	// Check the current local route
	if(!tg->gwg->CheckTradeRoute(current_pos,local_route,local_pos,tg->player))
	{
		// Not valid, recalc it
		return RecalcLocalRoute();
	}

	unsigned char next_dir = local_route[local_pos];

	// Next step
	if(++local_pos >= local_route.size())
	{
		local_pos = 0;
		++global_pos;
		RecalcLocalRoute();
	}

	return next_dir;

}

/// Recalc local route and returns next direction
unsigned char TradeRoute::RecalcLocalRoute()
{
	unsigned char next_dir = tg->gwg->FindTradePath(current_pos,tg->GetNode(current_pos_tg).main_pos,tg->player,TG_PF_LENGTH,
		false,&local_route);
	if(next_dir != 0xff)
		return next_dir;
	else
		return RecalcGlobalRoute();
}

/// Recalc the whole route and returns next direction
unsigned char TradeRoute::RecalcGlobalRoute()
{
	// TG node where we start
	Point<MapCoord> start_tgn = TradeGraphNode::ConverToTGCoords(start);
	// Try to calc paths to the main point and - if this doesn't work - to the mainpoints of the surrounded nodes
	unsigned char next_dir;
	for(unsigned char i = 0;i<=8;++i)
	{
		// Try to find path
		start_tgn = tg->GetNodeAround(current_pos_tg,i);
		next_dir = tg->gwg->FindTradePath(current_pos,tg->GetNode
			(start_tgn).main_pos,tg->player,TG_PF_LENGTH,
			false,&local_route);
		// Found a path? Then abort the loop
		if(next_dir != 0xff)
			break;
	}

	// Didn't find any paths? Then bye
	if(next_dir == 0xff)
		return 0xff;
	
	// The same for the last path main_point-->destination
	// TG node where we end
	Point<MapCoord> goal_tgn = TradeGraphNode::ConverToTGCoords(goal);
	// Try to calc paths to the main point and - if this doesn't work - to the mainpoints of the surrounded nodes
	for(unsigned char i = 0;i<=8;++i)
	{
		// Try to find path
		goal_tgn = tg->GetNodeAround(current_pos_tg,i);
		next_dir = tg->gwg->FindTradePath(tg->GetNode
			(goal_tgn).main_pos,goal,tg->player,TG_PF_LENGTH,
			false);
		// Found a path? Then abort the loop
		if(next_dir != 0xff)
			break;
	}
	// Didn't find any paths? Then bye
	if(next_dir == 0xff)
		return 0xff;


	// Pathfinding on the TradeGraph
	if(!tg->FindPath(start_tgn,goal_tgn,global_route))
		return 0xff;

	return local_route[0];
}


/// Returns to coordinate of the node around this node 
/// (Directions 1-8 (incl), 0 = no change)
Point<MapCoord> TradeGraph::GetNodeAround(const Point<MapCoord> pos, const unsigned char dir) const
{
	Point<int> cpos = Point<int>(pos.x,pos.y);
	Point<int> new_pos(cpos);
	switch(dir)
	{
	case 1: new_pos = Point<int>(cpos.x-1,cpos.y); break;
	case 2: new_pos = Point<int>(cpos.x-1,cpos.y-1); break;
	case 3: new_pos = Point<int>(cpos.x,cpos.y-1); break;
	case 4: new_pos = Point<int>(cpos.x+1,cpos.y-1); break;
	case 5: new_pos = Point<int>(cpos.x+1,cpos.y); break;
	case 6: new_pos = Point<int>(cpos.x+1,cpos.y+1); break;
	case 7: new_pos = Point<int>(cpos.x-1,cpos.y+1); break;
	case 8: new_pos = Point<int>(cpos.x-1,cpos.y+1); break;
	default: break;
	}

	// Consider map borders
	if(new_pos.x < 0) new_pos.x += size.x;
	if(new_pos.x >= size.x) new_pos.x -= size.x;
	if(new_pos.y < 0) new_pos.y += size.y;
	if(new_pos.y >= size.y) new_pos.y -= size.y;

	return Point<MapCoord>(new_pos.x,new_pos.y);
}

struct TGN
{
	unsigned route_length; /// Length of the route consisting of "turn" directions
	unsigned real_length; /// Length of the route in real steps on the map
	unsigned char dir;
	bool visited;
	bool in_list;

	TGN() : route_length(0),real_length(0),visited(false),in_list(false) {}
};



/// Find a path on the Trade Graph
bool TradeGraph::FindPath(const Point<MapCoord> start, const Point<MapCoord> goal, std::vector<unsigned char>& route) const
{
	// Todo list
	std::list< Point<MapCoord> > todo;
	todo.push_back(start);

	std::vector<TGN> nodes(size.x*size.y);

	unsigned nodes_count = 0;

	while(todo.size())
	{
		// Liste leer und kein Ziel erreicht --> kein Weg
		if(!todo.size())
			return 0;

		unsigned shortest_route = 0xFFFFFFFF;

		std::list<Point<MapCoord>>::iterator best_it;

		for(std::list<Point<MapCoord>>::iterator it = todo.begin(); it!=todo.end(); ++it)
		{
			unsigned new_way = nodes[it->y*size.x+it->x].real_length + TGN_SIZE;
			if(new_way < shortest_route)
			{
				shortest_route = new_way;
				best_it = it;
			}
		}

		nodes[best_it->y*size.x+best_it->x].visited = true;


		for(unsigned i = 0;i<8;++i)
		{
			if(GetNode(*best_it).dirs[i] == oo)
				continue;

			Point<MapCoord> new_pos(GetNodeAround(*best_it,i+1));

			if(nodes[new_pos.y*size.x+new_pos.x].visited)
				continue;

			if(new_pos == goal)
			{
				// Reached goal
				route.resize(nodes[best_it->y*size.x+best_it->x].route_length);
				route[route.size()-1] = i;

				Point<MapCoord> pos = *best_it;

				for(int z = route.size()-1;z>=0;--z,pos = GetNodeAround(pos,nodes[pos.y*size.x+pos.x].dir))
					route[z] =nodes[pos.y*size.x+pos.x].dir;

				return true;
			}

			unsigned new_length = nodes[best_it->y*size.x+best_it->x].real_length + GetNode(*best_it).dirs[i];

			if(new_length < nodes[new_pos.y*size.x+new_pos.x].real_length)
			{
				nodes[new_pos.y*size.x+new_pos.x].real_length = new_length;
				nodes[new_pos.y*size.x+new_pos.x].dir = i;
				nodes[new_pos.y*size.x+new_pos.x].route_length = nodes[best_it->y*size.x+best_it->x].route_length+1;
				if(!nodes[new_pos.y*size.x+new_pos.x].in_list)
				{
					nodes[new_pos.y*size.x+new_pos.x].in_list = true;
					todo.push_back(new_pos);
				}
			}
		}

		// Knoten behandelt --> raus aus der todo Liste
		todo.erase(best_it);

	}

	return false;
}


/// Finds a main point for a speciefic node
void TradeGraph::FindMainPoint(const Point<MapCoord> tgn)
{
	/// Calc size of this node rectangle
	MapCoord width,height;
	if(tgn.x == gwg->GetWidth()/TGN_SIZE || gwg->GetWidth()%TGN_SIZE == 0)
		width = gwg->GetWidth()%TGN_SIZE;
	else
		width = TGN_SIZE;
	if(tgn.y == gwg->GetHeight()/TGN_SIZE || gwg->GetHeight()%TGN_SIZE == 0)
		height = gwg->GetHeight()%TGN_SIZE;
	else
		height = TGN_SIZE;

	const unsigned POTENTIAL_MAIN_POINTS = 5;


	// We consider the following points as main points
	Point<MapCoord> ps[POTENTIAL_MAIN_POINTS] =
	{
		Point<MapCoord>(tgn.x*TGN_SIZE+width/2,tgn.y*TGN_SIZE+height/2),

		Point<MapCoord>(tgn.x*TGN_SIZE+width/4,tgn.y*TGN_SIZE+height/4),
		Point<MapCoord>(tgn.x*TGN_SIZE+width*3/4,tgn.y*TGN_SIZE+height/4),
		Point<MapCoord>(tgn.x*TGN_SIZE+width*3/4,tgn.y*TGN_SIZE+height*3/4),
		Point<MapCoord>(tgn.x*TGN_SIZE+width/4,tgn.y*TGN_SIZE+height*3/4)
	};

	bool good_points[POTENTIAL_MAIN_POINTS];

	for(unsigned i = 0;i<POTENTIAL_MAIN_POINTS;++i)
	{
		Point<MapCoord> p = ps[i];
		good_points[i] =gwg->IsNodeForFigures(p.x,p.y);

		// Valid point? Otherwise choose one around this one
		if(!gwg->IsNodeForFigures(p.x,p.y))
		{
			for(unsigned d = 0;d<6;++d)
			{
				Point<MapCoord> pt(gwg->GetPointA(p,d));
				if(gwg->IsNodeForFigures(pt.x,pt.y))
				{
					ps[i] = pt;
					good_points[i] = true;
					break;
				}
			}
		}
	}

	// Try to find paths to the other points if we reach at least 3/4 of the other points, choose 
	// this point at once, otherwise choose the point with most connections
	unsigned best_connections = 0, best_id = 0;
	for(unsigned i = 0;i<POTENTIAL_MAIN_POINTS;++i)
	{
		Point<MapCoord> p = ps[i];
		unsigned connections = 0;
		for(unsigned j = 0;j<POTENTIAL_MAIN_POINTS;++j)
		{
			if(i==j || !good_points[j]) continue;

			unsigned char next_dir = gwg->FindTradePath(p,ps[j],player,TG_PF_LENGTH,false,NULL,false);
			if(next_dir != 0xff) ++connections;
		}

		if(connections >= (POTENTIAL_MAIN_POINTS-1)*3/4)
		{
			best_id = i;
			break;
		}
		
		if(connections >= best_connections)
		{
			best_id = i;
			best_connections = connections;
		}
	}

	GetNode(tgn).main_pos = ps[best_id];

}


/// Updates one speciefic edge
void TradeGraph::UpdateEdge(Point<MapCoord> pos, const unsigned char dir, const TradeGraph * const tg)
{
	if(tg)
		if(tg->GetNode(pos).dont_run_over_player_territory[dir])
		{
			GetNode(pos).dont_run_over_player_territory[dir] = true;
			GetNode(pos).dirs[dir] = tg->GetNode(pos).dirs[dir];
			return;
		}
	Point<MapCoord> other = GetNodeAround(pos,dir+1);
	unsigned char other_dir = (dir+4)%8;
	if(GetNode(other).dont_run_over_player_territory[other_dir])
	{
		GetNode(pos).dont_run_over_player_territory[dir] = true;
		GetNode(pos).dirs[dir] = GetNode(other).dirs[other_dir];
		return;
	}


	unsigned length;
	std::vector<unsigned char> route;
	Point<MapCoord> mpos(GetNode(pos).main_pos);
	// Simply try to find a path from one main point to another
	if(gwg->FindTradePath(mpos,GetNode(other).main_pos,
		player,TG_PF_LENGTH,false,&route,&length) == 0xff)
		length = oo;
	GetNode(pos).dirs[dir] = length;

	bool player = false;
	for(unsigned i = 0;i<route.size();++i)
	{
		mpos = gwg->GetPointA(mpos,route[i]);
		if(gwg->GetNode(mpos.x,mpos.y).owner != 0)
			player = true;
	}

	if(!player)
		GetNode(pos).dont_run_over_player_territory[dir] = true;

}