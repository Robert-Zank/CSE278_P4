// Copyright Robert Zank 2023
/*
 * It connects to a MySQL database, executes a SELECT SQL statement, and displays the results.
 * Code inspired by: Z.Sevkli
 */

// Including necessary libraries
#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <iomanip>
#include "getpost.h"

// Using specific namespaces for readability
using std::cout;
using std::cerr;
using std::endl;

// Function to print HTML header
void PageHTMLHeader() {
    // Printing HTML and CSS styles for formatting
    cout << "Content-type: text/html" << endl << endl;
    cout << "<html> <head> <meta charset=\"UTF-8\">" << endl;
    cout << "<style> body { " <<
            "font-family: \"Arial\", sans-serif;" <<
            "background-color: #3c5b2e;" <<
            " max-width: 600pt;" <<
            " padding: 72pt;} " <<
        // CSS styles for table and text
        "table {" <<
            "margin: 0 auto;" << 
            "background-color: #883a65; " <<
            "border-spacing: 0;" <<
            "border-collapse: collapse;" <<
            "width: 100%; " <<
       " } .title { color: #ffffff; text-align:center; font-size: 26pt; font-weight: 700; margin-bottom: 3pt;}" <<
       ".nav-links {color: #d5d0d0; font-weight: 700; text-decoration: none; text-align: center;}" <<
       "</style></head>" << endl;

    // Start of HTML body
    cout << "<body>" << endl;
    // Header section for navigation
    cout << "<table> " <<
        "<tr>" <<
            "<td>" <<
                "<p class=\"title\">An Order Query Application</p>" <<
                "<p class=\"nav-links\">" << 
                "<a href=\"http://os1.csi.miamioh.edu/~zankrj/OrdersQueryApplicationLinks.html\"> QueryPage </a>" <<
                " | <a href=\"http://os1.csi.miamioh.edu/~zankrj/cgi-html/dropdownQuery.cgi\"> Enter Another Query </a> </p>" <<
            "</td>" <<
        "</tr>" <<
    "</table>" << endl;
}

// Main function
int main(int argc, char *argv[]) {
    PageHTMLHeader();

    // Create a map object to access form objects
    std::map<std::string, std::string> Get;
    initializePost(Get);

    float customerNumber;
    
    if (Get.find("dropdown") != Get.end()) {
        customerNumber = std::stof(Get["dropdown"]); // Convert string to float (PROBLEMATIC AREA)
        cout << "<h2 style=\"color: white;\"> Here is the result for customerNumber = " << customerNumber << "</h2>" << endl;
    } else {
        cout << "<p style=\"color: white;\">Fill out the form and press submit"<<endl;
    }
    // Connect to the database
    mysqlpp::Connection conn; // Exceptions suppressed to keep code simple
    if (conn.connect("cse278_Fall2023", "localhost", "cse278_Fall2023", "rubrikPenguin")) {
        // Create a query
        mysqlpp::Query query = conn.query();
        query << "SELECT Count(*) a ,status FROM Orders WHERE customerNumber = " << customerNumber << " GROUP BY status;";

        // Check the query for correctness
        query.parse();

        // Run the query and get the result
        mysqlpp::StoreQueryResult result = query.store();

        // Check for errors and display results
        if (result.num_rows() > 0) {
            // Display result in a table
            cout << "<table border= \"1\" width=\"300\">";
            // Display headers
            cout << "<th style=\"color: white;\"> status </th>"
                << "<th style=\"color: white;\"> Number of Orders </th>" << endl;

            // Get each row in the result set and print its contents
            for (int i = 0; i < result.num_rows(); ++i) {
                cout << "<tr>" <<
                        "<td style=\"color: white;\">" << result[i]["status"] << "</td>" <<
                        "<td style=\"color: white;\">" << result[i]["a"] << "</td>" << "</tr>" << endl;
            }
            cout << "</table>" << endl;
            cout << "</body></html>" << endl;
        } else {
            cout << "<p style=\"color: white;\"> Query does not return any records" << query.error() << "</p>" << endl;
            cout << "</body></html>" << endl;
        }
        return 0;
    } else {
        cout << "<p style=\"color: white;\"> Connection failed: " << conn.error() << "</p>" << endl;
        cout << "</body></html>" << endl;
        return 1;
    }
}
