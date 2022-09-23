import random
import numpy as np
import sys

def test_gen (out_dir, n_tests):

    for test_i in range (n_tests):
        capacity = random.randint(10, 1000)
        requests = random.randint(1001, 10000)

        gamma = np.random.gamma(1, 150, requests)
        binomial = np.random.binomial(100, 0.2, requests)

        with open(f"./{out_dir}/test_{test_i}", "w") as gen_test:
            
            gen_test.write(str(capacity) + " " + str(requests * 2) + "\n")

            for g, b in zip (gamma, binomial):
                gen_test.write(f"{int(g)} {int(b)} ")
            gen_test.write("\n")

out_dir = str(sys.argv[1])
n_tests = int(sys.argv[2])
test_gen(out_dir, n_tests)
