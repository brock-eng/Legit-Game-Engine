#pragma once

#include <iostream>

/*
   Scrapped templated matrix class
*/

namespace legit_engine {
   namespace components {

      template <typename T, int R, int C>
      class matrix
      {
      private:
         T body[R][C];
         int rowSize = R;
         int collumnSize = C;

      public:

         matrix()
         {
            for (int r = 0; r < rowSize; r++)
            {
               for (int c = 0; c < collumnSize; c++)
               {
                  body[r][c] = NULL;
               }
            }
         }

         matrix(T inputData[R][C])
         {
            for (int r = 0; r < rowSize; r++)
            {
               for (int c = 0; c < collumnSize; c++)
               {
                  body[r][c] = inputData[r][c];
               }
            }
         }

         matrix& multiply() {}

         void print()
         {
            for (int r = 0; r < rowSize; r++)
            {
               std::cout << "|";
               for (int c = 0; c < collumnSize; c++)
               {
                  std::cout << body[r][c] << " ";
               }
               std::cout << "|" << std::endl;
            }
         }

         std::string toString()
         {
            std::string output = "";
            for (int r = 0; r < rowSize; r++)
            {
               output += "| ";
               for (int c = 0; c < collumnSize; c++)
               {
                  T num = body[r][c];
                  output += num;
                  output += " ";
               }
               output += "|\n";
            }
            return output;
         }
         
         void TestMatrices()
         {
            float arrayTest[4][5] = { {4, 3, 2, 3, 5}, {1, 2, 3, 4, 6}, {5, 6, 7, 8, 9}, {10, 11, 12, 13, 14} };
            matrix<float, 4, 5> m1(arrayTest);

            m1.print();
         }

      };
} }