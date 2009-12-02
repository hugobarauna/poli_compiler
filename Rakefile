require 'rake/clean'
CLEAN.include('*.o')
CLOBBER.include('bin/test:*')

ARGS = "-std=c99 -Wall -O2 -g"
SRC = FileList['**/*.c']

require "ruby-debug"

###################
#   Test Tasks   ##
###################

namespace "test" do |namespace|

  desc "Run StringBuffer tests"
  task :string_buffer => ["string_buffer_test.o", "string_buffer.o", "error.o", 
                               "CuTest.o"] do |t|
      generated_test_file = make_tests('test/string_buffer_test.c')
      sh "gcc #{ARGS} -c #{generated_test_file}"
      sh "gcc #{ARGS} #{t.prerequisites.join(' ')} cu_string_buffer_test.o -o bin/#{t.name}"
      sh "./bin/#{t.name}"
  end
  
  desc "Run BufferedInputStream tests"
  task :bufstream => ["bufstream_test.o", "bufstream.o", "error.o", 
                               "CuTest.o"] do |t|
      generated_test_file = make_tests('test/bufstream_test.c')
      sh "gcc #{ARGS} -c #{generated_test_file}"
      sh "gcc #{ARGS} #{t.prerequisites.join(' ')} cu_bufstream_test.o -o bin/#{t.name}"
      sh "./bin/#{t.name}"
  end
  
  desc "Run Alloc module tests"
  task :alloc => ["alloc_test.o", "alloc.o", "error.o", 
                               "CuTest.o"] do |t|
      generated_test_file = make_tests('test/alloc_test.c')
      sh "gcc #{ARGS} -c #{generated_test_file}"
      sh "gcc #{ARGS} #{t.prerequisites.join(' ')} cu_alloc_test.o -o bin/#{t.name}"
      sh "./bin/#{t.name}"
  end
  
  desc "Run eXtension string module tests"
  task :xstring => ["xstring_test.o", "xstring.o", "error.o",
                                "CuTest.o"] do |t|
      generated_test_file = make_tests('test/xstring_test.c')
      sh "gcc #{ARGS} -c #{generated_test_file}"
      sh "gcc #{ARGS} #{t.prerequisites.join(' ')} cu_xstring_test.o -o bin/#{t.name}"
      sh "./bin/#{t.name}"
  end

  desc "Run FiniteAutomata tests"
  task :fa => ["fa_test.o", "fa.o", "bufstream.o", "alloc.o", "error.o",
                  "xstring.o", "CuTest.o"] do |t|
      generated_test_file = make_tests('test/fa_test.c')
      sh "gcc #{ARGS} -c #{generated_test_file}"
      sh "gcc #{ARGS} #{t.prerequisites.join(' ')} cu_fa_test.o -o bin/#{t.name}"
      sh "./bin/#{t.name}"
  end

  desc "Run lexer tests"
  task :lexer => ["lexer_test.o", "lexer.o", "string_buffer.o", "error.o", 
                  "bufstream.o", "xstring.o", "alloc.o", "hashtable.o", "CuTest.o"] do |t|
      generated_test_file = make_tests('test/lexer_test.c')
      sh "gcc #{ARGS} -c #{generated_test_file}"
      sh "gcc #{ARGS} #{t.prerequisites.join(' ')} cu_lexer_test.o -o bin/#{t.name}"
      sh "./bin/#{t.name}"
  end

  desc "Run hashtable tests"
  task :hashtable => ["hashtable_test.o", "hashtable.o", "CuTest.o"] do |t|
      generated_test_file = make_tests('test/hashtable_test.c')
      sh "gcc #{ARGS} -c #{generated_test_file}"
      sh "gcc #{ARGS} #{t.prerequisites.join(' ')} cu_hashtable_test.o -o bin/#{t.name}"
      sh "./bin/#{t.name}"
  end
  
  desc "Run parser tests"
  task :parser => ["parser_test.o", "parser.o", "lexer.o", "string_buffer.o", "error.o", 
                   "bufstream.o", "xstring.o", "alloc.o", "hashtable.o", "CuTest.o",
                   "stack.o", "semantic_actions.o", "scope.o"] do |t|
    generated_test_file = make_tests('test/parser_test.c')
    sh "gcc #{ARGS} -c #{generated_test_file}"
    sh "gcc #{ARGS} #{t.prerequisites.join(' ')} cu_parser_test.o -o bin/#{t.name}"
    sh "./bin/#{t.name}"
  end
  
  desc "Run stack tests"
  task :stack => ["stack_test.o", "stack.o", "CuTest.o"] do |t|
      generated_test_file = make_tests('test/stack_test.c')
      sh "gcc #{ARGS} -c #{generated_test_file}"
      sh "gcc #{ARGS} #{t.prerequisites.join(' ')} cu_stack_test.o -o bin/#{t.name}"
      sh "./bin/#{t.name}"
  end
  
  desc "Run semantic actions tests"
  task :semantic_actions => ["semantic_actions_test.o", "semantic_actions.o", "CuTest.o",
                             "hashtable.o", "stack.o", "lexer.o", "string_buffer.o", "error.o", 
                             "bufstream.o", "xstring.o", "alloc.o", "scope.o"] do |t|
      generated_test_file = make_tests('test/semantic_actions_test.c')
      sh "gcc #{ARGS} -c #{generated_test_file}"
      sh "gcc #{ARGS} #{t.prerequisites.join(' ')} cu_semantic_actions_test.o -o bin/#{t.name}"
      sh "./bin/#{t.name}"
  end
  
  desc "Run scope tests"
  task :scope => ["scope_test.o", "scope.o", "CuTest.o",
                   "hashtable.o", "error.o"] do |t|
      generated_test_file = make_tests('test/scope_test.c')
      sh "gcc #{ARGS} -c #{generated_test_file}"
      sh "gcc #{ARGS} #{t.prerequisites.join(' ')} cu_scope_test.o -o bin/#{t.name}"
      sh "./bin/#{t.name}"
  end

  test_tasks = namespace.tasks.inject([]) { |tasks, task| tasks.concat(task.prerequisites) }
  test_tasks.delete("clean")
  test_tasks.uniq!
  # This task must be defined after all tests
  desc "Run all tests"
  task :all => test_tasks do |t|
    generated_test_file = make_tests
    sh "gcc #{ARGS} -c #{generated_test_file}"
    sh "gcc #{ARGS} #{t.prerequisites.join(' ')} cu_all_test.o -o bin/#{t.name}"
    sh "./bin/#{t.name}"
  end
end

#####################
#   Compilations   ##
#####################

#file "hashtable.o" => ["src/hashtable.c"]

########################################
#   Tests tasks  dependencies         ##
########################################

file "CuTest.o"               => ["test/CuTest.c"]
file "bufstream_test.o"       => ["test/bufstream_test.c", "src/bufstream.c"]
file "alloc_test.o"           => ["test/alloc_test.c", "src/alloc.c"]
file "xstring_test.o"         => ["test/xstring_test.c", "src/xstring.c"]
file "fa_test.o"              => ["test/fa_test.c", "src/fa.c"]
file "lexer_test.o"           => ["test/lexer_test.c", "src/lexer.c"]
file "string_buffer_test.o"   => ["test/string_buffer_test.c", "src/string_buffer.c"]
file "hashtable_test.o"       => ["test/hashtable_test.c", "src/hashtable.c"]
file "parser_test.o"          => ["test/parser_test.c", "src/parser.c"]
file "stack_test.o"           => ["test/stack_test.c", "src/stack.c"]
file "semantic_actions.o"     => ["test/semantic_actions_test.c", "src/semantic_actions.c"]
file "scope.o"                => ["test/scope_test.c", "src/scope.c"]



##############
#   Rules   ##
##############

# This rule generate rake tasks for the scenarios where you want to generate
# a object file that can have source files that aren't in the same directory of 
# the object file.
#
rule '.o' => lambda{ |objfile| find_deps(objfile) } do |t|
  deps = t.prerequisites.join(' ')
  sh "gcc #{ARGS} -c #{deps}"
end


########################
#   Helpers methods   ##
########################


# This methods finds all the source files that a CuTest unit test object file 
# can depends on.
# For example:
#   find_deps('hashtable_test.o')
#     => ['src/hashtable.c', 'test/hashtable_test.c']
#
def find_deps(object_file)
  base = File.basename(object_file, '.o')

  SRC.find_all do |source| 
    File.basename(source, '.c') == base || 
        File.basename(source, '.c') == base.sub(/_test/, '')
  end
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

def retrieve_tests(test_file)
  test_names = []
  f = File.open(test_file, "r")
  f.each_line do |line|
    test_names << line.gsub(/(^void\s*|\(.*$\n?)/, '') if line =~ /^void test_/
  end
  f.close
  test_names
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
