#!/usr/bin/perl
use strict;
use Term::ANSIColor;
use warnings;
use Time::HiRes qw(usleep ualarm gettimeofday tv_interval);
use feature qw(switch);
#global 
my $ip_rest = "127.0.0.1";
my $port_rest = 1234;
my $CURL="curl ";#-m 1";
#function  
sub SendQuery { # arg  0 HTTP_METHOD         # arg  1 url 
        print color("on_yellow"),"------ request is : -----", color("reset");print ("\n");
        print "$CURL -X $_[0]   http://$ip_rest:$port_rest/$_[1] \n";
        print color("on_yellow"),"------ response is : -----", color("reset");print ("\n");   
my $t0 = [gettimeofday];     
        my $result = `$CURL -X $_[0]   http://$ip_rest:$port_rest/$_[1]  2>&1 2>/dev/null `;
my $elapsed = tv_interval ($t0);	
        print "$elapsed:sec >>> $result <<<<";
        print ("\n");print color("on_yellow"),"-------------------------", color("reset");print ("\n");
        return $result;
}
#function
sub SendEventing{ # NO Args 	 
  my $MET="GET";
  my $URL="root/eventing";  
  print ">>  Asking for $MET  @@ root/eventing  \n";
  print color("on_yellow"),"------ request is : -----", color("reset");print ("\n");
  print "$CURL -X $MET   http://$ip_rest:$port_rest/$URL \n";
  print color("on_yellow"),"------ response is : -----", color("reset");print ("\n");
my $t0 = [gettimeofday];          
  my $result = ` $CURL -X $MET  http://$ip_rest:$port_rest/$URL  2>&1 2>/dev/null `;
my $elapsed = tv_interval ($t0);
  print "$elapsed:sec >>> $result <<<<";
  print ("\n");print color("on_yellow"),"-------------------------", color("reset");print ("\n");		
}
#function 
sub SendAleatory { # NO Args 
        my $handlers_d="nothing,root,root/security,root/authentication,root/pairing,root/security/koi,root/pairing/ispair,root/pairing/error,root/pairing/pairMe,root/error,root/eventing";
        my $methods_d="POST,GET,DELETE,PUT,OPTIONS";
        my @handlers = split(',', $handlers_d);
        my @methods = split(',', $methods_d);
        my $count=0;
        my @results={};
my $t00 = [gettimeofday];  
        foreach my $handler (@handlers) 
        {
                foreach my $method (@methods) 
                {
                        print "Asking for $handler using $method \n";
                        $results[$count] = SendQuery($method,$handler);
                        $count++;
                }
          }
my $elapsed0 = tv_interval ($t00);
        #Results  
        $count = 0;
        print color("green")," ==================================== \n", color("reset");
	 foreach my $handler (@handlers) 
        {
		foreach my $method (@methods) 
                {
			print color("blue"),"<$handler>\t\t\t[$method]\t\t$results[$count]\n", color("reset");			
			$count++;
		}
	}
	print color("green"),"$elapsed0:sec for $count query ==================================== \n", color("reset");
}
#Main
while (1) {
print color("green")," ==================================== ", color("reset");
print color("green"),"  [Services Expo V2.0] Choose option  ", color("reset");
print color("green")," ==================================== \n", color("reset");
print color("blue")," 1. ## POST     ##\n", color("reset");
print color("blue")," 2. ## GET      ##\n", color("reset");
print color("blue")," 3. ## DELETE   ##\n", color("reset");
print color("blue")," 4. ## PUT      ##\n", color("reset");
print color("blue")," 5. ## OPTIONS  ##\n", color("reset");
print color("blue")," 6. ## ALEATORY ##\n", color("reset");
print color("blue")," 7. ## Eventing ##\n", color("reset");
print color("blue")," 0. Finish\n", color("reset");

my $userinput=  <STDIN>;
chomp ($userinput);
print color("cyan"),"User typed $userinput\n", color("reset");
given($userinput){
  when(1) { 
                        print color("magenta"),"POST ,Put URL:\n", color("reset");
                        my $userURL=  <STDIN>;
                        chomp ($userURL);
                        SendQuery("POST",$userURL);
                  }
  when(2) { 
                        print color("magenta"),"GET ,Put URL:", color("reset");
                        my $userURL=  <STDIN>;
                        chomp ($userURL);
                        SendQuery("GET",$userURL);
                   }
  when(3) {
                        print color("magenta"),"DELETE ,Put URL:", color("reset");
                        my $userURL=  <STDIN>;
                        chomp ($userURL);
                        SendQuery("DELETE",$userURL);
                   }
  when(4) {
                        print color("magenta"),"PUT ,Put URL:", color("reset");
                        my $userURL=  <STDIN>;
                        chomp ($userURL);
                        SendQuery("PUT",$userURL);
                    }
  when(5) {     
                        print color("magenta"),"OPTIONS ,Put URL:", color("reset");
                        my $userURL=  <STDIN>;
                        chomp ($userURL);
                        SendQuery("OPTIONS",$userURL);
                   }
  when(6){		 SendAleatory(); }
  when(7){		 SendEventing(); }
  when(0){
                        print color("green")," ====================================", color("reset");
                        print color("green"),"  [Services Expo V2.0] Bye Bye", color("reset");
                        print color("green")," ====================================\n ", color("reset");
                        system "killall curl"; 
                        exit 0;
                 }
  default { 
                        print color("red"),"Veuillez taper un numéro de 0-6\n", color("reset");
                }
}
        
}

