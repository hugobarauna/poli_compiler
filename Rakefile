ARGS = "-std=c99 -Wall -O2"

# Tasks

desc "Compile hashtable"
task :hashtable => "bin/hashtable.o"

desc "Run scanner test"
task :scanner_test => "test/scanner_test.o" do |t|
    generated_test_file = make_tests('test/scanner_test.c')
    deps = prepare_dependencies(generated_test_file)
    generated_test = generated_test_file.split('.')[0]
    sh "gcc #{ARGS} #{deps} src/scanner.c test/scanner_test.c -o bin/#{generated_test} && ./bin/#{generated_test}"
end

desc "Run lexer test"
task :lexer_test => "test/lexer_test.o" do |t|
    generated_test_file = make_tests('test/lexer_test.c')
    deps = prepare_dependencies(generated_test_file)
    generated_test = generated_test_file.split('.')[0]
    sh "gcc #{ARGS} #{deps} src/lexer.c test/lexer_test.c -o bin/#{generated_test} && ./bin/#{generated_test}"
end

# compilations

file "bin/hashtable.o" => ["src/hashtable.c"]

# tests
file "test/scanner_test.o" => ["test/scanner_test.c", "src/scanner.c"]
file "test/lexer_test.o" => ["test/lexer_test.c", "src/lexer.c"]

# Rules

# generate object files
rule '.o' => '.c' do |t|
  deps = t.prerequisites.join(' ')
  sh "gcc #{ARGS} -c #{deps}"
end

# Helper Functions

def prepare_dependencies(prerequisites=[])
  deps = []
  deps << prerequisites
  deps << ["test/CuTest.c"]
  deps.join(' ')
end

def retrieve_tests(test_file)
  test_names = []
  f = File.open(test_file, "r")
  f.each_line do |line|
    test_names << line.gsub(/(^void\s*|\(.*$\n?)/, '') if line =~ /^void test_/
  end
  f.close
  test_names
end

def make_tests(test_file=nil)
  test_names = []
  output_file_name = ''
  if not test_file.nil?
    output_file_name = "#{test_file.gsub(/test\//, 'test/cu_')}"
    test_names = retrieve_tests(test_file)
  else
    output_file_name = "test/cu_all_test.c"
    Dir["test/*.c"].each do |f|
      test_names.concat(retrieve_tests(f))
    end
  end

  function_declarations = ''
  test_registrations = ''
  test_names.each do |test_name|
    function_declarations << "extern void #{test_name}(CuTest*);\n"
    test_registrations << "    SUITE_ADD_TEST(suite, #{test_name});\n"
  end
  
  code = TEST_CODE_TEMPLATE
  code.sub!(/%fuction_declarations%/, function_declarations)
  code.sub!(/%tests_registrations%/, test_registrations)
  
  f = File.new(output_file_name, "w+")
  f.write(code)
  f.close
  
  output_file_name
end


TEST_CODE_TEMPLATE = <<EOF
#include <stdio.h>

/* This is auto-generated code. Edit at your own peril. */

#include "CuTest.h"

%fuction_declarations%

void RunAllTests(void) 
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

%tests_registrations%

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\\n", output->buffer);
}

int main(void)
{
    RunAllTests();
    return 0;
}
EOF

