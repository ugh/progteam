
//COMPILING & RUNNING: javac Main.java && java Main < input.txt 

import java.util.*;

public class Main
{
	public static final String ERDOS = "Erdos, P.";
    public static Scanner in;
 
    public static void main(String[] args) 
	{
        in = new Scanner(System.in);

        doStuff();
    }

    public static void doStuff() 
	{
        int numPapers = in.nextInt();
		int numNames = in.nextInt();
		in.nextLine();
		
		int database = 1;

		while (numPapers != 0 && numNames != 0) {
			System.out.println("Database #" + database);
			solve(numPapers, numNames);
			numPapers = in.nextInt();
			numNames = in.nextInt();
			in.nextLine();
			database++;
        }
    }

	public static void solve(int numPapers, int numNames) 
	{
		//System.out.println("num papers : " + numPapers + ", numNames: " + numNames);
		Map<String, Set<String>> graph = new HashMap<String, Set<String>>();

		for (int i = 0; i < numPapers; i++) {
			addToGraph(graph, in.nextLine());
		}

		/*
		System.out.println("Graph: ");
		for ( String n : graph.keySet() ) {
			System.out.println(n+":");
			for ( String friend : graph.get(n) ) {
				System.out.println("   " + friend);
			}
		}
		*/

        Set<String> names1 = new HashSet<String>();
        List<String> names2 = new LinkedList<String>();

		for (int i = 0; i < numNames; i++) {
			String name = in.nextLine().trim();
            names1.add(name);
            names2.add(name);
        }

		Map<String, Integer> distMap = erdos(graph, names1);

		for (String name : names2) {
			if (distMap.containsKey(name)) {
				System.out.println(name + ": " + distMap.get(name));
			}
			else {
				System.out.println(name + ": infinity");
			}
		}
		System.out.println();
    }

	public static void addToGraph(Map<String, Set<String>> graph, String paperEntry) 
	{
		String[] namesAndPaper = paperEntry.split(":");
		if (namesAndPaper.length != 2) {
			return;
		}
		
		List<String> names = getNameList(namesAndPaper[0]);

		for (String name : names) {
			Set<String> adjList;
			if (graph.containsKey(name)) {
				adjList = graph.get(name);
			}
			else {
				adjList = new HashSet<String>();
				graph.put(name, adjList);
			}

			for (String otherName : names) {
				if (name == otherName) {
					continue;
				}
				if (!adjList.contains(otherName)) {
					adjList.add(otherName);
				}
			}
		}
	}


	public static List<String> getNameList(String nameStr)
	{
		List<String> names = new LinkedList<String>();

		int start = 0;
		int end = nameStr.indexOf(',', start);
		if (end == -1) {
			return names;
		}
		end = nameStr.indexOf(',', end+1);

		while ( end != -1 ) {
			names.add(nameStr.substring(start, end).trim());
			
			start = end+1;
			end = nameStr.indexOf(',', start);
			if (end == -1) {
				return names;
			}
			end = nameStr.indexOf(',', end+1 );
		}

		String lastName = nameStr.substring(start).trim();
		if (lastName != "") {
			names.add(lastName);
		}

		return names;
	}


    // update so that it takes another param, a list of names to find.
    // have the routine return early if it finds all the names
	public static Map<String, Integer> erdos(Map<String, Set<String>> graph,
                                             Set<String> namesToFind)
	{
		Map<String, Integer> distMap = new HashMap<String, Integer>();

		if (!graph.containsKey(ERDOS)) {
			return distMap;
		}

		distMap.put(ERDOS, 0);

		Queue<String> namesToVisit = new LinkedList<String>();
		namesToVisit.offer(ERDOS);

		while (namesToVisit.peek() != null) {
			String currentName = namesToVisit.poll();
			//			System.out.println("Current Name: " + currentName);
			int dist = -1;
			if (distMap.containsKey(currentName))
				dist = distMap.get(currentName);
			else {
				System.out.println("[ERROR] Name not found in distMap: " + currentName);
				return null;
			}

			for ( String child : graph.get(currentName) ) {
				if (!distMap.containsKey(child)) {
					distMap.put(child, dist + 1);
					namesToVisit.offer(child);

                    if (namesToFind.contains(child)) {
                        namesToFind.remove(child);
                        if (namesToFind.isEmpty()) {
                            return distMap;
                        }
                    }
				}
			}
		}
		return distMap;
	}



}


