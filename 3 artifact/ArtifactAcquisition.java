import java.util.Arrays;
import java.util.Scanner;

public class ArtifactAcquisition {
    public static void main(String[] args) {
        // --------------------
        // Variable declaration
        // --------------------

        Scanner sc;
        int n, m, q, ind;
        DisjointSet ds;
        Event[] paths, queries;
        boolean[] results;

        // -------------------
        // Variable assignment
        // -------------------
        sc = new Scanner(System.in);

        // Number of nodes/pedestals.
        n = sc.nextInt();
        // Number of edges/paths.
        m = sc.nextInt();

        // A disjoint set containing n nodes/pedestals.
        ds = new DisjointSet(n);
        // An Event array of m edges/paths.
        paths = new Event[m];

        // Reads in the edges/paths.
        for (int i = 0; i < m; i++) {
            paths[i] = new Event(sc.nextInt()-1, sc.nextInt()-1, sc.nextInt(), -1);
        }

        // Number of queries/archaeologists.
        q = sc.nextInt();

        // An Event array of q queries/archaeologists.
        queries = new Event[q];
        // A boolean array containing the results of each query.
        results = new boolean[q];

        // Reads in the queries/archaeologists.
        for (int i = 0; i < q; i++) {
            queries[i] = new Event(sc.nextInt()-1, sc.nextInt()-1, sc.nextInt(), i);
        }

        // Keeps track of location in paths array for later (See constructAdditionalPaths method).
        ind = 0;

        sc.close();

        // ---------------
        // Offline Queries
        // ---------------

        // Sort paths and queries by weight.
        Arrays.sort(paths);
        Arrays.sort(queries);

        // Compute queries
        for (int i = 0; i < results.length; i++) {
            // Connect possible paths(See constructAdditionalPaths method).
            ind = constructAdditionalPaths(ds, paths, ind, queries[i].weight);

            // If the start and end points are in the same group, there is a possible path between them.
            // Store result of the query in the original order they were asked.
            results[queries[i].query_index] = ds.find(queries[i].start) == ds.find(queries[i].end);
        }

        // -------------
        // Print Queries
        // -------------

        for (boolean result : results) {
            System.out.println(result?"Yes":"No");
        }
    }

    // ==================
    // HELPER FUNCTION(S)
    // ==================

    // Input(s):
    //     ds: disjoint set containing nodes/pedestals
    //     paths: Event array containing paths/edges
    //     ind: Location of where we are in the array
    //     weight: the weight of the artifact 
    // Description:
    //     Similar to Kruscal's algorithm, we go through an array of edges sorted by least weight
    //     and join the closest different group of nodes. The difference is that we loop through
    //     the edges that can hold the weight of the artifact and then stop since we know the rest 
    //     of the edges can not be crossed. To take advantage of the Offline query trait, we use a
    //     disjoint set already connected up until the previous weight, and use an index variable
    //     to start at that point in the paths array to obtain a near linear runtime. we then return
    //     where we left off at in the paths array for later use.
    private static int constructAdditionalPaths(DisjointSet ds, Event[] paths, int ind, int weight) {
        for (int i = ind;  i < paths.length; i++) {
            if (paths[i].weight < weight) {
                ind = i;
                break;
            }

            if (ds.find(paths[i].start) != ds.find(paths[i].end)) {
                ds.merge(paths[i].start, paths[i].end);
            }
        }
        return ind;
    }

    // =================
    // DATA STRUCTURE(S)
    // =================

    private static class Event implements Comparable<Event> {
        int start, end, weight;
        int query_index;

        public Event(int start, int end, int weight, int query_index) {
            this.start = start;
            this.end = end;
            this.weight = weight;
            this.query_index = query_index;
        }

        // Used to sort elements of an Event array in descending order.
        public int compareTo(Event o) {
            return Integer.compare(o.weight, this.weight);
        }
    }

    private static class DisjointSet {
        private int[] nodes;

        public DisjointSet(int size) {
            this.nodes = new int[size];
            fill();
        }

        // Input(s):
        //     None
        // Description:
        //     Sets the index that each cell points to, to itself.
        private void fill() {
            for (int i = 0; i < nodes.length; i++) {
                nodes[i] = i;
            }
        }

        // Input(s):
        //     n: the index of a represented node
        // Description:
        //     Looks for who the head representative of the node is. If there is
        //     a chain of nodes before finding the head, set those nodes' head
        //     to the head representative
        private int find(int n) {
            if (this.nodes[n] == n) return n;
            this.nodes[n] = find(this.nodes[n]);
            return this.nodes[n];
        }

        // Input(s):
        //     i: index of a node from one group
        //     j: index of a node from a different group
        // Description:
        //     Merges the groups i and j belong to by setting
        //     the head of one node to the other
        private void merge(int i, int j) {
            nodes[find(j)] = find(i);
        }
    }
}
