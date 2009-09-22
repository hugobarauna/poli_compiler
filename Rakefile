file "bin/hashtable.o" => ["src/hashtable.c", "src/hashtable.h"] do
  sh "gcc -c -o bin/hashtable.o src/hashtable.c"
end

desc "Compile hashtable"
task :hashtable => "bin/hashtable.o"


#file "bin/test.o" => ["src/test.c", "src/test.h"] do
#  sh "gcc -c -o bin/test.o src/test.c"
#end

#file 'bin/main.o' => ["src/main.c", "src/test.h"] do
#  sh "gcc -c -o bin/main.o src/main.c"
#end

#file 'bin/main' => ['bin/test.o', 'bin/main.o'] do
#  sh "gcc -o bin/main bin/main.o bin/test.o"
#end

#desc "Compile tests"
#task :compile => 'bin/main'
