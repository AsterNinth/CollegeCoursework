import java.util.Arrays;
import java.util.Scanner;

public class DesertOasis {
    public static int a, b;

    public static void main(String[] args) {
        // Our way of getting information from standard input
        Scanner sc = new Scanner(System.in);

        // Take in the first line of variables
        int n = sc.nextInt();
        a = sc.nextInt();
        b = sc.nextInt();

        // Create an array and store each set of variables in each line as an object in said array
        Friend[] friends = new Friend[n];

        for (int i = 0; i < n; i++) {
            friends[i] = new Friend(sc.nextInt(), sc.nextInt(), sc.next());
        }

        // Sort the array and print the order
        Arrays.sort(friends);

        for (Friend f: friends) {
            System.out.println(f.name);
        }

        // generally a good idea to close scanner when finished with it
        sc.close();
    }

    // implements Comparable to make sorting an array of objects easier
    public static class Friend implements Comparable<Friend>{
        public int x, y;
        public String name;

        public Friend(int x, int y, String name) {
            this.x = x;
            this.y = y;
            this.name = name;
        }

        public long getDistance() {
            // longs are used and returned since the max possible distance (oasis is -100000, -100000
            // and the person is at 100000, 100000 or similar) exceeds the limitations of an integer
            long x_diff = (this.x-a);
            long y_diff = (this.y-b);

            x_diff *= x_diff;
            y_diff *= y_diff;

            // Since what we really want are distances relative to each other, we can ignore taking the square
            // root of the value since it keeps the order of people who are close to the point the same.
            return x_diff+y_diff;
        }

        // implemented from Comparable interface
        // sorts from least to greatest in terms of relative distance
        public int compareTo(Friend o) {
            return Long.compare(getDistance(), o.getDistance());
        }
    }
}
