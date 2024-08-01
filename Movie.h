/**
 * Name: Haiyu Huang, Kyle Chang
 * Date: November 13, 2016
 * CSE 100 Fall 2016 
 * PA4: Graph Theory 
 * File: Movie.h
 * Description: Struct definition for Movie object.
 */ 


#ifndef MOVIE_H
#define MOVIE_H

using namespace std;

/**
 * Struct to define the member variables and methods related to Movie objects
 * used in Pathfinder.
 *
 */ 
struct Movie{
    string movie_title; // title of movie
    int movie_year;     // year of movie
    //Movie(string title, int year):
    //    movie_title(title), movie_year(year){}

    /**
     * Overloaded == operator to compare Movie objects
     */ 
    bool operator == (const Movie & other ) const{
        return (movie_title == other.movie_title) && (movie_year == other.movie_year);
    }
};

/**
 * Custom hash function for Movie class for the map
 */ 
namespace std {

  template <>
  struct hash<Movie>
  {
    std::size_t operator()(const Movie& k) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      // Compute individual hash values for movie title
      // and movie year, and combine them using XOR
      // and bit shifting:

      return (hash<string>()(k.movie_title))
               ^ (hash<int>()(k.movie_year) << 1);
    }
  };

}
#endif //MOVIE_H
