#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>   // std::vector
#include <cstddef>  // std::ptrdiff_t

/// <b> Struck </b> Token <i> To save symbol and type </i>
struct Token
{
    //!   Enumerator to  save type.
    enum class token_type : int
    {
        OPERAND = 0,
        OPERATOR,
        OPEN_SCOPE,
        CLOSE_SCOPE,
        POWER_OPERATOR,
    };

    std::string value; //!< string to save valor of numbers.
    token_type type; //!< Save type ( OPERAND / OPERATOR / OPEN_SCOPE / CLOSE_SCOPE / POWER_OPERATOR )

    //! Constructor Token Without Miss an Term.
    explicit Token( std::string value_in = "", token_type type_in = token_type::OPERAND )
        : value{value_in} , type{type_in} //
    {/* Empty */}
};
/// <b> Class </b> Parser <i> to Valid a expression and to separate in token on vector. </i>
class Parser
{
    public:
        /// <b> Struct </b> ResultType To Save Result type and place when is the erro.
        struct ResultType
        {

            typedef std::ptrdiff_t size_type; //!< Use to arithmetic pointer.

            /// ! Enumerator to save Type Code
            enum code_type{ 
                OK = 0,
                INCOMPLETE_EXPRE,
                IMCOMPLETE_INT,
                MISS_TERM,
                WRONG_SYMBOL,
                DIV_ZERO,
                MISS_CLOSE_SCOPE,
                INT_OUT_RANGE,
            };

            code_type type; //!< Result type 
            size_type col; //!< Place when is the erro.

            /// ! Constructor ResultType Without Miss an Term.
            explicit ResultType( code_type type_ = OK , size_type col_ = 0u)
                :type{ type_ } , col{ col_ }
            {/* Empty */}
        };
        //! Recive an string and convert the term to Token.
        /*!
          \param std:string expre_ Expression to be Token.
          \return ResultType A struct with result of Token.
          */
        ResultType parser( std::string expre_ );
        //! Is a get method to see the list of Token.
        /*! .
          \return std::vector< Token > List of the token.
          */
        std::vector< Token > get_tokens( void ) const;
        /// ! Constructor Parser .
        Parser() = default;
        /// ! Destructor Parser .
        ~Parser() = default;
        /// ! Copy Parser .
        Parser( const Parser & ) = delete;

    private:

        /// ! Enumerator synbol_type to save Symbol.
        enum class symbol_type
        {
            ST_SUM,
            ST_DIF,
            ST_PROD,
            ST_QUOT,
            ST_MOD,
            ST_POWER,
            ST_OPEN_SCOPE,
            ST_CLOSE_SCOPE,
            ST_ZERO,
            ST_DIGIT,
            ST_SPACE,
            ST_TAB,
            ST_END,
            ST_INVALID
        };

        std::string m_expr; //!< String to save expression
        std::string::iterator m_point_expr; //!<  A pointer on string to walks in the string 
        std::vector< Token > m_token_list; //!<  vector withe list of Token
        ResultType m_result;    //!< Result of expression

        //! Check if the 'x' is a symbol
        /*!
          \param Char 'x' character to be check.
          \return symbol_type if valid our not.
        */
        symbol_type get_symbol( char x ) const;

        /// ! Jump to next symbol in the string.
        void next_symbol( void );
        //! Check if the Symbol_type is acceptable.
        /*!
          \param Symbol_type x symbol_type to be check.
          \return True if symbol is valid .
          \return False if Symbol is invalid.
        */
        bool accept( symbol_type x );
        /// ! Jump White Space until to find a symbol our end_string.
        void next_space( void );
        //! check if the m_token_expr is on the end.
        /*!
          \return True if m_token_expr is m_expr.end().
          \return False Otherwise.
        */
        bool end_string( void ) const;

        /*======Grammar======*/
        //! Check if the m_token_expr is '0'.
        /*!
          \return True if m_token_expr is '0'.
          \return False Otherwise.
        */
        bool digit_zero( void );
        //! Check if the m_token_expr is (0, 9].
        /*!
          \return True if the m_token_expr is (0 , 9].
          \return False Otherwise.
        */
        bool digit( void );
        //! Check if the m_token_expr is [0, 9]
        /*!
          \return True if the m_token_expr is [0 , 9].
          \return False Otherwise.
        */
        bool natural( void );
        //! Check if the m_token_expr is an integer.
        /*!
          \return True if the m_token_expr  is (-∞, +∞).
          \return False Otherwise
        */
        bool integer( void );
        //! Check if the term is acceptable.
        /*!
          \return True  {'(' + expression() + ')'} or interger().
          \return False Otherwise
        */
        bool term( void );
        //! Check if the expression is acceptable.
        /*!
          \return True {term() + symol_tyope and term() One or more}.
          \return False Otherwise
        */
        bool expression( void );
};
#endif
