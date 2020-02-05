#include <iostream>
#include <iomanip>
#include <fstream>

#include "../include/infix_to_postfix.h"
#include "../include/parser.h"

/*! \mainpage BARES Index Page
 * \section intro_sec Introduction
 *
 * **Wellcome to BARES Project**
 *
 * This is a Bares Program: recive an expression Parse in TOKENS valid the expression 
 * them change to infix to postfix and give the result.
 *
 * \section install_sec Run
 *
 * \subsection step1 Step 1: Make ./bares
 *
 * g++ -Wall -std=c++11 src/main.cpp src/parser.cpp src/infix_to_postfix.cpp -I include/ -o bares
 *
 * \subsection step2 Step 2: Run ./bares
 * ./keno > saida.txt \n
 *
 *
*/
std::vector<std::string> expressions =
{
    " 2 ^ 8",
    "2 ^ ( 4 * 2 )",
    " 2  ^ 4  * 2",
    " 2 + 3 * 40",
    "(2+3) * 40",
    "-2",
    "-3 * -5",
    "5 * -3",
    "------5",
    "-------5",
    "--3 * ----1",
    "10 ^ 2",
    "100 ^ 50",
    "2 +   2 ^ 4",
    "10 - 3 ^3",
    "10 % 2 - 4",
    "4 - 16 % 4",
    "1 % 3",
    "240 / 12",
    "12 / 240",
    "120 / (2- 12 % 10 )",
    " 2 * 4",
    " 10 * 23",
    " 10000 * 50 * 30 * 2 * 5",
    " 30 * 2 * 5",
    " 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9",
    " 1 + 2 + 3 + 4 + 5 + 6 + 7",
    " 1 + 2 - 2 + 3 -3 + 4 - 4 + 5 -5 - 1",
    " 35 - 3 * (-2 + 5) ^2",
    " 54 / 3  ^(12%5) * 2",
    " ((2-3)*10 - (2^3*5))",
    "    2^3%4*5+32-(540/9)",
    "10000000  - 2",
    "   23 * 4 + 50000000",
    "   ---40000000 * 3 + 41",
    "    (3*4) / (5 * 20000000)",
    "-  5",
    "5 ^",
    "5 /",
    "5 *",
    "5  %",
    "5  -",
    "5  +",
    "1 + 2 ^",
    "1 + 2 /",
    "1 + 2 *",
    "1 + 2 %",
    "1 + 2 -",
    "1 + 2 +",
    "2 = 3",
    "2.3 * 4",
    "2 @ 4",
    "2 & 4",
    "4 $",
    "! 1",
    "@ 3",
    "# 4",
    "2 + 3 4",
    "2 - 3 4",
    "2 * 3 4",
    "2 / 3 4",
    "2 % 3 4",
    "( 1 + 2 ) ( 3 * 4 )",
    "1 ( 2 * 300)",
    "(300 + 1 ) - ( 2 )",
    ") 20 - 40",
    "20 ) - 40",
    "30  / ) 40",
    "20 + 40 )",
    "( ( 11 % 3 ) * 4 ",
    "( 2 * 3 ) + ( 40 % 2",
    "( 2 + ( 4 - ( 5 * ( 6 / ( 7 % 3",
    "( 2 + ( 4 - ( 5 * ( 6 / ( 7 % 3 )",
    "( 2 + ( 4 - ( 5 * ( 6 / ( 7 % 3 ) + 1 )",
    "( 2 + ( 4 - ( 5 * ( 6 / ( 7 % 3 ) + 1 ) + 1 )",
    "( 2 + ( 4 - ( 5 * ( 6 / ( 7 % 3 ) + 1 ) + 1 ) + 1 )",
    "( 2 + ( 4 - ( 5 * ( 6 / ( 7 % 3 ) + 1 ) + 1 ) + 1 ) + 1 )",
    "( ( ( ( 3 * 4 ) * 2 ) * 2 ) * 2 ) / 4",
    "30 / ( 1 - 1 )",
    "10 / ( 3 * 3^-2 )",
    "10 / ( 3 * 0 )",
    "(3 * 0 ) / 2",
    "20 * 20000",
    "30000 / 5 * 500",
    "1 + 2 ^ 64 ",
    "1a * 3",
    "      ",
    "    (",

};

std::vector<std::string> expr_out =
{ };

void print_error_msg( const Parser::ResultType & result, std::string str )
{
    std::string error_indicator( str.size()+1, ' ');

    // Have we got a parsing error?
    error_indicator[result.col] = '^';
    switch ( result.type )
    {
        case Parser::ResultType::MISS_CLOSE_SCOPE:
            std::cout << ">>> Missing closing ) at column (" << result.col+1 << ")!\n";
            break;
        case Parser::ResultType::INCOMPLETE_EXPRE:
            std::cout << ">>> Unexpected end of input at column (" << result.col+1 << ")!\n";
            break;
        case Parser::ResultType::IMCOMPLETE_INT:
            std::cout << ">>> Ill formed integer at column (" << result.col+1 << ")!\n";
            break;
        case Parser::ResultType::MISS_TERM:
            std::cout << ">>> Missing <term> at column (" << result.col+1 << ")!\n";
            break;
        case Parser::ResultType::WRONG_SYMBOL:
            std::cout << ">>> Extraneous symbol after valid expression found at column (" 
                << result.col+1 << ")!\n";
            break;
        case Parser::ResultType::INT_OUT_RANGE:
            std::cout << ">>> Integer constant out of range beginning at column (" 
                << result.col+1 << ")!\n";
            break;
        case Parser::ResultType::DIV_ZERO:
            std::cout << ">>> Division by zero (" 
                << result.col+1 << ")!\n";
            break;
        default:
            std::cout << ">>> Unhandled error found!\n";
            break;

    }

    // std::cout << "\"" << str << "\"\n";
    // std::cout << " " << error_indicator << std::endl;
}
int main( void )
{
     //! @brief This is a Bares program
    /*!  @author     Bergony Bandeira
     *
     *   @date 01/11/2018
     *
    */
    std::ifstream fin( "docs/entrada.txt" );              //Stream opem in fin
    infix_to_postfix fix_post; // Creates a convert
    std::string expr_in;
    while( fin.eof() == false )
    {
        std::getline( fin, expr_in );
        expr_out.push_back( expr_in );
    }
    expr_out.pop_back(); //Remove the last line Trash
    fin.close();       //stream close
    Parser my_parser; // Create a Parser

    for( const auto & expr : expr_out )
    {
        auto result = my_parser.parser( expr ); // Parser the expr.
        fix_post.set_m_token_list( my_parser.get_tokens() ); // Pass to token to convert.

        // ==================== Header ================================
        // std::cout << std::setfill('=') << std::setw(80) << " ";
        // std::cout << std::setfill(' ') << "\n>>> Parsing \"" << expr << "\" \n";
        if ( result.type != Parser::ResultType::OK )
            print_error_msg( result, expr ); // Printer Error
        else
        {

            auto list = my_parser.get_tokens(); // Save tokens in list
            // ==================== Passer Printer =======================
            //std::cout << ">>> Expression SUCCESSFULLY parsed!\n";
            /*std::cout << ">>> Parser Tokens:\n{ ";
              for(unsigned int x{0}; x < list.size(); x++)
              {
              std::cout << "[ " << list.at(x).value << ", ";
              if( list.at(x).type == Token::token_type::OPERAND)
              {
              std::cout << " Operand";
              }else if( list.at(x).type == Token::token_type::OPERATOR)
              {
              std::cout << " Operator";
              }else if( list.at(x).type == Token::token_type::OPEN_SCOPE)
              {
              std::cout << " Open Scope";
              }else if( list.at(x).type == Token::token_type::CLOSE_SCOPE)
              {
              std::cout << " Close Scope";
              }else if( list.at(x).type == Token::token_type::POWER_OPERATOR)
              {
              std::cout << " Power Operator";
              }

              std::cout << " ] ";
              }*/
            list = fix_post.convert();
            //=================== Parser Postfix ========================
            /* std::cout << "\n>>> Postfix Tokens:\n{ ";
               for(unsigned int x{0}; x < list.size(); x++)
               {
               std::cout << "[ " << list.at(x).value << ", ";
               if( list.at(x).type == Token::token_type::OPERAND)
               {
               std::cout << " Operand";
               }else if( list.at(x).type == Token::token_type::OPERATOR)
               {
               std::cout << " Operator";
               }else if( list.at(x).type == Token::token_type::OPEN_SCOPE)
               {
               std::cout << " Open Scope";
               }else if( list.at(x).type == Token::token_type::CLOSE_SCOPE)
               {
               std::cout << " Close Scope";
               }else if( list.at(x).type == Token::token_type::POWER_OPERATOR)
               {
               std::cout << " Power Operator";
               }

               std::cout << " ] ";
               }*/

            std::cout << fix_post.evaluate() << "\n"; // printer Result
        }
    }
    //==================== END Header ===========================
    // std::cout << "\n>>> Normal exiting...\n";

    return 0;
}
