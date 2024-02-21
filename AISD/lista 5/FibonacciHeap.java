import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class FibonacciHeap {
    public int operationComparisons = 0;

    private static class FibonacciTree {
        int value;
        List<FibonacciTree> child;
        int order;

        FibonacciTree(int value) {
            this.value = value;
            this.child = new ArrayList<>();
            this.order = 0;
        }

        void addAtEnd(FibonacciTree t) {
            child.add(t);
            order++;
        }
    }

    public List<FibonacciTree> trees;
    private FibonacciTree least;
    private int count;

    public FibonacciHeap() {
        this.trees = new ArrayList<>();
        this.least = null;
        this.count = 0;
    }

    public void insert(int value) {
        FibonacciTree newTree = new FibonacciTree(value);
        trees.add(newTree);
        operationComparisons++;
        if (least == null || value < least.value) {
            least = newTree;
        }
        count++;
        System.out.println("Insert " + value);
    }

    public Integer extractMin() {
        FibonacciTree smallest = least;
        if (smallest != null) {
            for (FibonacciTree child : smallest.child) {
                trees.add(child);
            }
            trees.remove(smallest);
            if (trees.isEmpty()) {
                least = null;
            } else {
                least = trees.get(0);
                consolidate();
            }
            count--;
            System.out.println("delete " + smallest.value);
            return smallest.value;
        }
        System.out.println("heap is empty");
        System.exit(0);
        return null;
    }

    private void consolidate() {
        int maxOrder = (int) (Math.log(count) / Math.log(2)) + 1;
        List<FibonacciTree> aux = new ArrayList<>();
        for (int i = 0; i <= maxOrder; i++) {
            aux.add(null);
        }

        while (!trees.isEmpty()) {
            FibonacciTree x = trees.get(0);
            int order = x.order;
            trees.remove(x);

            while (aux.get(order) != null) {
                FibonacciTree y = aux.get(order);
                operationComparisons++;
                if (x.value > y.value) {
                    FibonacciTree temp = x;
                    x = y;
                    y = temp;
                }
                x.addAtEnd(y);
                aux.set(order, null);
                order++;
            }
            aux.set(order, x);
        }

        least = null;
        for (FibonacciTree k : aux) {
            if (k != null) {
                trees.add(k);
                operationComparisons++;
                if (least == null || k.value < least.value) {
                    least = k;
                }
            }
        }
    }

    public void merge(FibonacciHeap otherHeap) {
        trees.addAll(otherHeap.trees);
        operationComparisons++;
        if (least == null || (otherHeap.least != null && otherHeap.least.value < least.value)) {
            least = otherHeap.least;
        }
        count += otherHeap.count;
    }

    public static void main(String[] args) {
        FibonacciHeap fibonacciHeap = new FibonacciHeap();
        FibonacciHeap fibonacciHeap2 = new FibonacciHeap();
        Random ran = new Random();
        int nxt;
        System.out.println("H1");
        for(int i = 0; i<10;i++) {
            nxt = ran.nextInt(100000);
            fibonacciHeap.insert(nxt);
        }
        System.out.println("H2");
        for(int i = 0; i<10;i++) {
            nxt = ran.nextInt(100000);
            fibonacciHeap2.insert(nxt);
        }
        System.out.println("Merge");
        fibonacciHeap.merge(fibonacciHeap2);
        System.out.println("extracting");
        while (true) {
            fibonacciHeap.extractMin();
        }

    }
}
