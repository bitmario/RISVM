int main() {
    int num;
    int i;
    int isPrime;
    int primeCount;
    primeCount = 0;
    num = 2;

    while (num < 100000) {
        isPrime = 1;
        i = 2;

        while (i < num) {
            if (num % i == 0) {
                isPrime = 0;
                break;
            }
            ++i;
        }
        if (isPrime) {
            ++primeCount;
        }

        ++num;
    }
    return primeCount;
}