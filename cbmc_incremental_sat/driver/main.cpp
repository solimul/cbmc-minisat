/* Part of the generic incremental SAT API called 'ipasir'.
 *
 * This LICENSE applies to all software included in the IPASIR distribution,
 * except for those parts in sub-directories or in included software
 * distribution packages, such as tar and zip files, which have their own
 * license restrictions.  Those license restrictions are usually listed in the
 * corresponding LICENSE or COPYING files, either in the sub-directory or in
 * the included software distribution package (the tar or zip file).  Please
 * refer to those licenses for rights to use that software.
 *
 * Copyright (c) 2014, Tomas Balyo, Karlsruhe Institute of Technology.
 * Copyright (c) 2014, Armin Biere, Johannes Kepler University.
 * Copyright (c) 2015, Norbert Manthey
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */
#include <ipasir.h>
#include <stdlib.h>        // getenv
#include <unistd.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std::chrono;

std::string sig = std::string();

int main(int argc, char * argv[]) {
    printf("%s\n", ipasir_signature());
    
    std::string data;
    int lit;

    std::ifstream icnfFile(argv[1]);
    
    std::cout << "Solving " << argv[1] << " with " << ipasir_signature() << std::endl;
    void* solver = ipasir_init();
    while(icnfFile.is_open() && icnfFile >> data) {
        if (data.compare("c") == 0 || data.compare("p") == 0) {
            getline(icnfFile, data);
        }
        else if (data.compare("a") == 0) {
            int int_data;
            while (icnfFile >> int_data && int_data != 0) {
                ipasir_assume(solver, int_data);
            }
            ipasir_assume(solver, 0);

            std::cout << "Solving..." << std::endl;
            auto start = high_resolution_clock::now();
            int solve_result = ipasir_solve(solver);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            std::cout << "RESULT :"<< solve_result << " => " << ( (solve_result == 10) ? "SAT" : (solve_result == 20) ? "UNSAT" : "UNKNOWN STATUS" )<< std::endl;
            std::cout << "Solve Duration: " << duration.count() << " microseconds" << std::endl;
        }
        else {
            int int_data = std::stoi(data);

            do {
                ipasir_add(solver, int_data);
            } while (icnfFile >> int_data && int_data != 0);
            ipasir_add(solver, 0);

        }
    }
    ipasir_release(solver);
    
    return 0;
}