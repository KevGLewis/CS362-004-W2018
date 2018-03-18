

import junit.framework.TestCase;

//You can use this as a skeleton for your 3 different test approach
//It is an optional to use this file, you can generate your own test file(s) to test the target function!
// Again, it is up to you to use this file or not!

import java.util.Random;



public class UrlValidatorTest extends TestCase {


   public UrlValidatorTest(String testName) {
      super(testName);
   }

   public void customAssert(String statement, boolean boolOne, boolean boolTwo)
   {
      String assertionError = null;
      try {
         assertEquals(boolOne,boolTwo);
      }
      catch (AssertionError ae) {
         assertionError = ae.toString();
      }

      if(assertionError != null)
         System.out.println(statement + " " + assertionError);
   }

   public void testManualTest()
   {
//You can use this function to implement your manual testing
      // Set up the validator to accept all schemes
      UrlValidator urlVal = new UrlValidator();

      customAssert("http://www.google.aaab", false, urlVal.isValid("http://www.google.aaab"));

      //Test the custom scheme functionality
      String[] schemes = {"testone","xxxyyy"};
      UrlValidator urlValCustScheme = new UrlValidator(schemes);

      customAssert("testone://www.google.com", true,urlValCustScheme.isValid("testone://www.google.com"));
      customAssert("xxxyyy://www.google.com/", true,urlValCustScheme.isValid("xxxyyy://www.google.com/"));
      customAssert("http://www.google.com/", false,urlValCustScheme.isValid("http://www.google.com/"));

      // Set up a validator that also accepts two slashes and the Local URLs, testing these options
      UrlValidator urlValTwoSlash = new UrlValidator(null, null, UrlValidator.ALLOW_2_SLASHES + UrlValidator.ALLOW_LOCAL_URLS);

      customAssert("ftp://www.google.com/test//testfile", true,urlValTwoSlash.isValid("ftp://www.google.com/test//testfile"));

      String[] trueURLsTwoSlash = { // Typical
              "http://localhost/", // Typical with slash
              "http://machine/", // local address
              "http://localhost:8000", // with ports
              "ftp://localhost:8000",
              "http://machine:0",
              "http://www.google.com/test//testfile",
              "ftp://www.google.com/test//testfile",
              "ftp://www.google.com/file1//test2//test3"
      };

      // Insert the manual URls that we are going to test
      String[] trueURLS = {"http://www.google.com", // Typical
              "http://www.google.com/", // Typical with slash
              "http://0.0.0.0/", // local address
              "http://www.google.com/test", // With a
              "http://stackoverflow.com/", // No Domain
              "http://www.google.com/test/test/test/test/test/test/testFile", // A long domain with scheme
              "http://www.google.com/test/test/test/test/test/test/testFile", // A long domain with scheme
              "http://www.google.com/test?action=view",
              "http://www.google.com/#/",// Path option with symbol
              "http://www.google.com/23/",
              "http://www.google.ceo", // Several TLD
              "http://www.google.food",
              "http://www.google.com:0", // Good Ports
              "http://www.google.com:6000"
      };

      // List all of the URLs we are going to manually test
      String[] falseURLS = {"http://www.google.cmx/", // Bad TLD
              "http://www.google.aaab", // verify bad TLD
              "http://www.google.jdifj", // Checking different bad TLD cases
              "http://www.google.jdifj/",
              "http://www.google.jdifj/",
              "ftp://www.google.jdifj", // Checking different bad TLD cases
              "ftp://www.google.jdifj/",
              "ftp://www.google.jdifj/",
              "htp:/www.google.com/", // Incorrect Scheme Variations
              "http:/www.google.com/",
              "http:/www.google.com/#", //address with incorrect path option",
              "255.255.255.255", // No Scheme
              "http://256.256.256.256", // Impossible IP address
              "ftp://256.256.256.256",
              "http://www.google.com/..//file", // Testing dots in the file
              "http://www.google.com:-1", // Bad Ports
              "http://www.google.com:65a",
              "http://www.google.com/test//testfile", // Try two Slashes
              "http://localhost/", // Typical with slash
              "http://machine/", // local address
              "http://localhost:8000", // with ports
              "ftp://localhost:8000",
              "http://machine:0",
              "http://www.google.com/test//testfile",
              "ftp://www.google.com/test//testfile",
              "ftp://www.google.com/file1//test2//test3"
      };

      // Run through the true URLs
      for(int i = 0; i < trueURLS.length; i++)
      {
         customAssert(trueURLS[i], true, urlVal.isValid(trueURLS[i]));
      }

      // Run through the false URLs
      for(int i = 0; i < falseURLS.length; i++)
      {
         customAssert(falseURLS[i], false, urlVal.isValid(falseURLS[i]));
      }

      // Run through the true URLs with the two slash option
      for(int i = 0; i < trueURLsTwoSlash.length; i++)
      {
         customAssert(trueURLsTwoSlash[i], true, urlValTwoSlash.isValid(trueURLsTwoSlash[i]));
      }
   }


// NOTE: As currently written, many of the partition assertions below crash the test suite by
// throwing an Exception Initializer Error. This error is thrown due to a bug we discovered in
// URLValidatorInCorrect (all of these partition tests pass without error when tested against URLValidatorCorrect).
// This bug in the isValid() function causes a crash when it attempts to handle any scheme that is not 'http'.
// Therefore, these tests are commented out for now so that they do not crash the rest of the test suite.
/*
   public void testYourFirstPartition()
   {
      UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);

      // PARTITION (1): URL COMPONENTS IN CORRECT ORDER
      //		a) all components valid
      assertTrue(urlVal.isValid("http://www.google.com:80/test?action=view"));

      //		b) all components invalid
      assertFalse(urlVal.isValid("htp:///w.google.asjdckl:999999999//test??action=view"));

      //		c) some components valid some invalid
      assertFalse(urlVal.isValid("htttp://wwww.google.com:80//test?action=view"));

      //		d) some missing required components but rest are valid
      assertFalse(urlVal.isValid("http://google/test?action=view"));

      //		e) some missing components and rest are invalid
      assertFalse(urlVal.isValid("htttp:///google.com//test??action==view"));

      //		f) optional components missing but rest valid
      assertTrue(urlVal.isValid("http://www.google.com"));

      //		g) optional components missing and rest invalid
      assertFalse(urlVal.isValid("w.google.caosjdfioa;"));

      //		h) special case: 'file' allows empty authority
      assertTrue(urlVal.isValid("file://myfile?action=view"));
   }


   public void testYourSecondPartition(){
      UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);

      // PARTITION (2): URL COMPONENTS TESTED ARE IN THE INCORRECT ORDER
      //		a) all components valid
      assertFalse(urlVal.isValid(":80www.http://google.com/test?action=view"));

      //		b) all components invalid
      assertFalse(urlVal.isValid("w.google.htp:///asjdckl//test??action=view;:999999999"));

      //		c) some components valid some invalid
      assertFalse(urlVal.isValid(":80//test?action=view.htttp://wwww.google.com"));

      //		d) some missing required components but rest are valid
      assertFalse(urlVal.isValid("/test?action=viewhttp://google/"));

      //		e) some missing required components and rest are invalid
      assertFalse(urlVal.isValid("google//test??action==viewhtttp:///"));

      //		f) optional components missing but rest valid
      assertFalse(urlVal.isValid("com.www.googlehttp://"));

      //		g) optional components missing and rest invalid
      assertFalse(urlVal.isValid("caosjdfioa;.google.ww"));

      //		h) special case: 'file' allows empty authority
      assertFalse(urlVal.isValid("myfile?file://action=view"));
   }
*/


   public void testIsValid()
   {
      UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES );
      Random rand = new Random();

      for (int i = 0; i < 1000; i++) {
         // A bug causes the UrlValidator to crash with other schemes, so we will stick to http
         //String URL = URLHelpers.randomValidScheme(5);
         String URL = "http://";

         // 70% chance of putting a valid host name
         // 10% chance of putting a valid IPv4 address
         // 10% chance of putting a random string for TLD
         // 10% chance of putting a random string for host
         int hostRand = rand.nextInt(9);
         if (hostRand >= 2) URL += URLHelpers.randomValidHost(rand.nextInt(20)+1, rand.nextInt(5)+1);
         if (hostRand == 2) URL += "." + URLHelpers.randomString(rand.nextInt(5));
         else if (hostRand == 1) URL += URLHelpers.randomValidIP();
         else if (hostRand == 0) URL += URLHelpers.randomString(rand.nextInt(15));

         // Port numbers are a known bug from manual testing
         // 90% chance of no port
         // 5% chance of valid port
         // 5% chance of random string port
         int portRand = rand.nextInt(19);
         if (portRand == 0) URL += ":" + URLHelpers.randomValidPortNumber();
         else if (portRand == 1) URL += ":" + URLHelpers.randomString(5);

         // Multiple segments in path is known bug from manual testing
         // 50% will have no path
         // 40% will have 1 segment in path
         // 5% will have random string for path
         // 5% will have multiple segments in path
         int pathRand = rand.nextInt(19);
         if (pathRand == 0) URL += URLHelpers.randomValidPath(rand.nextInt(10)+2, rand.nextInt(5));
         else if (pathRand == 1) URL += "/" + URLHelpers.randomString(rand.nextInt(20));
         else if (pathRand < 10) URL += URLHelpers.randomValidPath(rand.nextInt(10)+2, 1);

         // 25% chance to put a valid query
         // 25% chance to put random string for query
         // 50% chance for no query
         int randQuery = rand.nextInt(4);
         if (randQuery == 0) URL += URLHelpers.randomValidQuery(rand.nextInt(30));
         else if (randQuery == 1) URL += "?" + URLHelpers.randomString(rand.nextInt(30));

         //System.out.println(URL);

         try {
            boolean expected = URLHelpers.isURLValid(URL);
            boolean actual = urlVal.isValid(URL);
            customAssert(URL, expected, actual);
            if (expected != actual) deltaDebug(URL);
         } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Caused exception: " + URL);
         }

      }
   }

   /*
      This shortens URLs that fail the assertion. The random tester can produce very long and hard to read
      URLs, so this takes out as many characters as possible without changing the result
    */
   public static void deltaDebug(String URL) {
      UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES );

      boolean expected = URLHelpers.isURLValid(URL);
      boolean actual = urlVal.isValid(URL);

      // Flag if there was at least 1 character taken out
      boolean flag = true;
      while (flag) {
         flag = false;
         int originalLength = URL.length();

         // Don't touch "http://" because it can cause exceptions and crashes
         for (int i = originalLength - 1; i >= 7; i--) {
            StringBuilder sb = new StringBuilder(URL);
            sb.deleteCharAt(i);
            String newURL = sb.toString();
            if (URLHelpers.isURLValid(newURL) == expected && urlVal.isValid(newURL) == actual) {
               URL = newURL;
               flag = true;
            }
         }
      }

      System.out.println("Delta debugger: " + URL);
   }



}
