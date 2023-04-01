#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <vector>
#include <random>


class progress_bar{

        std::vector<std::string> vs;

    public:

        int screen_width( ){
            struct winsize w;
            ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );
            int i = w.ws_col;
            return i;
        }

        void read_file( ){
            std::ifstream ifile;
            ifile.open( "list2" );
            std::string line;
            while( getline( ifile, line ) ){
                line += "-" + get_ver( );
                vs.push_back( line );
            }
        }

        std::string get_ver( ){
            std::default_random_engine gen( ( std::random_device( ) ) ( ) );
            std::uniform_int_distribution<int> dig( 0, 9 );
            std::uniform_int_distribution<int> sz( 2, 4 );
            int n = sz( gen );

            std::string numb = std::to_string( dig( gen ) );
            for( int i = 0; i < n; i++ ){
                numb += "." + std::to_string( dig( gen ) );
            }
            return numb;
        }

        void print_bar( const std::string name, const int perc, int sz ){
            std::cout << "\r\033[K";
            std::string progress = " [" + std::string( perc, '*' ) + std::string( sz - perc, ' ' ) + "]";
            int blank = screen_width( ) - name.size( ) - ( sz + 3 );
            std::cout << name << std::string( blank, ' ' ) << progress << "\033[F";
        }

        void bar( const std::string &name, const int sz ){
            for( int i = 0; i <= sz; i++ ){
                std::cout << std::endl;
                print_bar( name, i, sz );
                usleep( 10000 );
            }
        }

        void start( ){
            int k = screen_width( );
            for( int i = 0; i < vs.size( ); i++ ){
                std::string v = vs[ i ];
                bar( v, k * .5 );
                std::cout << "\033[F";
                std::cout << std::endl << std::endl;
                std::cout << v << "\033[K\033[F" << std::endl;
            }
        }
};

int main( int argc, char *argv[ ] ){
    progress_bar p;
    p.read_file( );
    std::cout << "\033\e[?25l";
    p.start( );
    std::cout << "\033\e[?25h";
    std::cout << std::endl << std::endl;
    return 0;
}

