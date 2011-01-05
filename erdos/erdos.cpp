
// ICPC Problem 1069
// Erdos Numbers

// TODO
// put this in git
// Read from standard in you idiot!


#include <stdlib.h>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

void parsePaper( std::string& line, map<string, list<string> >& graph )
{
    int startIdx = 0;
    int endIdx = 0;
    bool done = false;

    list<string> names;

    do
    {
        endIdx = line.find( ", ", line.find(", ", startIdx) + 1 );

        if ( endIdx == string::npos )
        {
            endIdx = line.find(":", startIdx);

            if ( endIdx != string::npos )
                done = true;
        }       

        string name( line.substr(startIdx, endIdx - startIdx) );

        names.push_back( name );

        startIdx = endIdx + 2;

//        cout << "PAPER name: " << name << endl;

    } while ( ! done ) ;

    list<string>::iterator nit;

    for (nit = names.begin(); nit != names.end(); ++nit)
    {
        string& name = *nit;

        map<string, list<string> >::iterator it = graph.find(name);
        if ( it == graph.end() )
        {
            graph[ name ] = list<string>();
        }
        it = graph.find(name);
        list<string>& peeps = it->second;

        list<string>::iterator nit2;

        for (nit2 = names.begin(); nit2 != names.end(); ++nit2)
        {
            if ( *nit2 != name 
                 && find( peeps.begin(), peeps.end(), *nit2 ) == peeps.end() )
            {
                peeps.push_back( *nit2 );
            }
        }
    }
}


int getDist(const string& vert, const map<string, int>& distMap)
{
    map<string, int>::const_iterator dit = distMap.find(vert);

    if ( dit == distMap.end() )
    {
        return -1;
    }
    else {
        return dit->second;
    }
}


string getClosest(const map<string, int>& distMap, 
                  list<string>& verts)
{
    string closest = "";
    int closestDist = -1;

    for (list<string>::iterator v = verts.begin(); v != verts.end(); ++v)
    {
        int dist = getDist(*v, distMap);

        if ( dist != -1 && (closestDist == -1 || dist < closestDist) )
        {
            closest = *v;
            closestDist = dist;
        }
    }
    return closest;
}


int erdos2( map<string, list<string> >& graph, const string& name )
{
    string THE_MAN( "Erdos, P." );

    if ( name == THE_MAN )
        return 0;

    map<string, list<string> >::iterator nit = graph.find( THE_MAN );

    if ( nit == graph.end() )
        return -1;

    list<string> neighbors = nit->second;

    map<string, int> distMap;
    for (list<string>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
        distMap[*it] = 1;
    
    list<string> toLookup( neighbors.begin(), neighbors.end() );

    while ( ! toLookup.empty() )
    {
        string& vert = toLookup.front();

        int dist = -1;
        map<string, int>::iterator distIt = distMap.find(vert);
        if ( distIt != distMap.end() )
        {
            dist = distIt->second;
        }

        if (vert == name || dist == -1)
            return dist;
        else
        {
            neighbors = graph.find( vert )->second;
            for (list<string>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
            {                
                distIt = distMap.find( *it );
                if ( distIt != distMap.end() )
                {
                    if ( distIt->second > dist + 1 )
                    {
                        distIt->second = dist + 1;
                    }
                }
                else
                {
                    distMap[ *it ] = dist + 1;
                    toLookup.push_back( *it );
                }
            }
        }
        toLookup.pop_front();
    }
    return -1;
}

/*
// dijkstra
int erdos( const map<string, list<string> >& graph, const string& name )
{
    map<string, int> distMap;

    distMap[ name ] = 0;

    list<string> verts;

    // get all the names(keys) from the graph
    for (map<string, list<string> >::const_iterator it = graph.begin();
         it != graph.end(); ++it)
    {
        verts.push_back( it->first );
    }

    int erdos = -1;

    while ( ! verts.empty() )
    {
        string closest = getClosest(distMap, verts);

        if ( closest == "")
            break; // all remaining verts are unreachable

        verts.remove(closest);

        int currentDist = getDist(closest, distMap);
        if ( currentDist == -1 ) 
        {
            cout << "ERROR Current dist not found for vert: " << closest << endl;
            break;
        }

        if ( closest == "Erdos, P." )
        {
            erdos = currentDist;
            break;
        }

        map<string, list<string> >::const_iterator it = graph.find(closest);
        if ( it == graph.end() )
        {
            cout << "ERROR Could not find vert in the graph: " << closest << endl;
            break;
        }
        const list<string>& neighbors = it->second;

        int altDist = currentDist + 1;

        for (list<string>::const_iterator n = neighbors.begin(); n != neighbors.end(); ++n)
        {
            int neighborDist = getDist(*n, distMap);

            if ( neighborDist == -1 || altDist < neighborDist )
            {
                map<string, int>::iterator dit = distMap.find(*n);
                if ( dit == distMap.end() )
                    distMap[*n] = altDist;
                else
                    dit->second = altDist;
            }
        } 
    }
    return erdos;
}
*/


void processDB( int papers, int names, fstream& file )
{
//    cout << "papers: " << papers << ", names: " << names << endl;

    string line;

    map<string, list<string> > graph;

    for ( int p = 0; p < papers; ++p ) 
    {
        getline( file, line );
//        cout << "paper: " << line << endl;
        parsePaper( line, graph );
    }

/*
    cout << "GRAPH" << endl;
    for (map<string, list<string> >::const_iterator it = graph.begin();
         it != graph.end(); ++it)
    {
        cout << " KEY: " << it->first << ", VALUE: " << endl;
        for (list<string>::const_iterator lit = it->second.begin();
             lit != it->second.end(); ++lit)
            cout << "  " << *lit << endl;
    }
*/


    for ( int n = 0; n < names; ++n ) 
    {
        getline( file, line );
        cout << line << ": ";
        // use graph to print numbers
        int dist = erdos2(graph, line);

        if (dist == -1)
        {
            cout << "infinity" << endl;
        }
        else
        {
            cout << dist << endl;
        }
    }
    cout << endl;

}


void readfile( const char* filename )
{
    fstream file( filename );
    string line;

    int dbNum = 1;

    while ( getline( file, line ) )
    {
        // get numbers
        int idx = line.find(" ");
        int papers = atoi( line.substr(0, idx).c_str() );
        
        int names = atoi( line.substr(idx+1).c_str() );

        if ( papers == 0 && names == 0 )
            return;

        cout << "Database #" << dbNum << endl;

        processDB( papers, names, file );

        ++dbNum;
    }
}


int main( int argc, char* argv[] )
{
    if ( argc != 2 )
        return 1;

    readfile( argv[1] );
    return 0;
}

