// Copyright Robert Zank 2023
/*
 * It connects to a MySQL database, executes a SELECT SQL statement, and displays the results.
 * Author: Robert Zank
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

    // Connect to the database
    mysqlpp::Connection conn; // Exceptions suppressed to keep code simple
    if (conn.connect("cse278_Fall2023", "localhost", "cse278_Fall2023", "rubrikPenguin")) {
        // Create a query
        mysqlpp::Query query = conn.query();
        query << "SELECT DISTINCT customerNumber FROM Orders ORDER BY customerNumber;";
        
        // Check the query for correctness
        query.parse();

        // Run the query and get the result
        mysqlpp::StoreQueryResult result = query.store();

        // Check for errors and display results
        if (result.num_rows() > 0) {
            // Display form for user interaction
            std::cout << "<p ></p>" << endl;
            std::cout << "<form action = \"dropdownQuery2.cgi\" method=\"POST\">" << endl;
            std::cout << "<fieldset> <legend style=\"color: white;\">Available Attributes</legend> " << endl;
            std::cout << "  <label style=\"color: white;\" for=\"dropdown\">Select a customerNumber: </label>\n";
            std::cout << "  <select id=\"dropdown\" name=\"dropdown\">\n";

            // Populate the dropdown menu with options from the database
            for (const auto& row : result) {
                std::cout << " <option value='" << row["customerNumber"] << "'>" << row["customerNumber"] << "</option>\n";
            }

            std::cout << "  </select>\n";
            std::cout << "  <input type=\"submit\" value=\"Submit\">\n";
            std::cout << "</fieldset>" << endl;
            std::cout << "</form>\n";

            cout << "</body></html>" << endl;
        } else {
            cout << "<p> Query does not return any records" << query.error() <<"</p>"<< endl;
            cout << "</body></html>" << endl;
        }
        return 0;
    } else {
        cout << "<p> Connection failed: " << conn.error() <<"</p>"<< endl;
        cout << "</body></html>" << endl;
        return 1;
    }
}
