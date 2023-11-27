
import java.util.*;

public class solution {

    // The main method
    public static void main(String[] Args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();

        // Read in the points
        Pair[] ps = new Pair[n];
        for (int i = 0; i < n; i++) {
            int d = sc.nextInt();
            int t = sc.nextInt();
            ps[i] = new Pair(d, t);
        }

        // Sort the points for sweeping purposes
        Arrays.sort(ps);

        // Create arrays to store the answer for the two quads
        long[] bigger = new long[n];
        long[] smaller = new long[n];

        // Create Fenwick Trees for the two quads
        int MAX = 1000010;
        BIT bit = new BIT(MAX);
        BIT bit2 = new BIT(MAX);

        // Sweep and compute the partial answers
        for (int i = 0; i < n; i++) {
            bigger[i] = bit.sum(ps[i].t);
            smaller[n-i-1] = bit2.sum(MAX-ps[n-i-1].t);
            bit.inc(ps[i].t,1);
            bit2.inc(MAX-ps[n-i-1].t,1);
        }

        // Build and print the answer
        long ans = 0;
        for (int i = 0; i < n; i++) {
            ans += bigger[i] * smaller[i];
        }
        System.out.println(ans);
    }

    // The Fenwick Tree Class
    public static class BIT {
        long[] arr;
        int n;
        BIT(int n) {
            arr = new long[this.n = n + 2];
        }
        long sum(int ind) {
            ind++;
            long ans = 0;
            while (ind != 0) {
                ans+=arr[ind];
                ind-=(ind&-ind);
            }
            return ans;
        }
        void inc(int ind, long val) {
            ind++;
            while (ind < n) {
                arr[ind] += val;
                ind+=(ind&-ind);
            }
        }
    }

    // Sortable 2D point class
    public static class Pair implements Comparable<Pair> {
        int d;
        int t;
        Pair(int d, int t) {
            this.d = d;
            this.t = t;
        }
        public int compareTo(Pair o) {
            return Integer.compare(d, o.d);
        }
    }
}
