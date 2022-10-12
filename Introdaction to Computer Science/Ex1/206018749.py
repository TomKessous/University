
import argparse
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--task', type=str, dest='task')
    parser.add_argument('--N', type=int, dest='number')

    arg = parser.parse_args()
    task = arg.task
    num = arg.number

    if num > 1000:
        print('wrong input')
        return 1
    elif num < 1:
        print('wrong input')
        return 1
    if task == 'pi':
        def calcPi(limit):  # Generator function
            """
            Prints out the digits of PI
            until it reaches the given limit
            """

            q, r, t, k, n, l = 1, 0, 1, 1, 3, 3

            decimal = limit
            counter = 0

            while counter != decimal + 1:
                if 4 * q + r - t < n * t:
                    # yield digit
                    yield n
                    # insert period after first digit
                    if counter == 0:
                        yield '.'
                    # end
                    if decimal == counter:
                        print('')
                        break
                    counter += 1
                    nr = 10 * (r - n * t)
                    n = ((10 * (3 * q + r)) // t) - 10 * n
                    q *= 10
                    r = nr
                else:
                    nr = (2 * q + r) * l
                    nn = (q * (7 * k) + 2 + (r * l)) // (t * l)
                    q *= k
                    t *= l
                    l += 2
                    k += 1
                    n = nn
                    r = nr


        # Calls CalcPi with the given limit
        pi_digits = calcPi(int(num))

        i = 0

        # Prints the output of calcPi generator function
        # Inserts a newline after every 40th number
        for d in pi_digits:
            print(d, end='')
            i += 1
            if i == 40:
                print("")
                i = 0
    elif task == 'prime':
        prime_list = [2]
        test_num = 3
        while len(prime_list) < num:
            for p in prime_list:
                if test_num % p == 0:
                    break
            else:
                prime_list.append(test_num)
            test_num += 2
        print(prime_list[-1])
    elif task == 'fibonacci':
        a = 1
        b = 1
        lst = [a, b]
        for i in range(2, num):
            U = b+a
            lst.append(U)
            b = lst[-1]
            a = lst[-2]

        fibonnaci_num = (lst[-1])
        fibonacci_str = (str(abs(fibonnaci_num)))
        counter = 0
        digits = ""
        for digit in fibonacci_str:
            digits += digit

            counter += 1
            if counter == 40:
                digits += "\n"
                counter = 0

        print(digits)

    else:
        print('wrong input')
        return 1


if __name__ == "__main__":
    main()
