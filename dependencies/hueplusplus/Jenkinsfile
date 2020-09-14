timestamps {
    node('master')
    {
        stage('SCM')
        {
            checkout scm
        }
        stage('Build')
        {
            sh returnStatus: true, script: 'rm -r build'
            sh '''#!/bin/bash
            mkdir build
            cd build
            cmake .. -Dhueplusplus_TESTS=ON
            make -j8 2>&1 | tee buildlog.txt
            test ${PIPESTATUS[0]} -eq 0'''
        }
        stage('Test')
        {
            sh '''cd build
            hueplusplus/test/test_HuePlusPlus --gtest_output=xml:test.xml
            make -j8 coveragetest'''
            step([$class: 'XUnitBuilder', testTimeMargin: '3000', thresholdMode: 1,
                thresholds: [
                    [$class: 'FailedThreshold', failureNewThreshold: '', failureThreshold: '', unstableNewThreshold: '', unstableThreshold: ''],
                    [$class: 'SkippedThreshold', failureNewThreshold: '', failureThreshold: '', unstableNewThreshold: '', unstableThreshold: '']
                    ],
                tools: [
                    [$class: 'GoogleTestType', deleteOutputFiles: true, failIfNotNew: true, pattern: 'build/test.xml', skipNoTestFiles: false, stopProcessingIfError: true]
                    ]
                ])
            publishHTML(
                [allowMissing: false, alwaysLinkToLastBuild: false, keepAll: false, reportDir: 'build/coveragetest', reportFiles: 'index.html', reportName: 'Coveragetest', reportTitles: '']
                )
        }
        stage('CppCheck')
        {
            sh 'cppcheck -j 8 --force -ihueplusplus/test -ihueplusplus/jsoncpp.cpp hueplusplus/ 2>build/CppCheckResult'
            rtp nullAction: '1', parserName: 'HTML', stableText: '${FILE:build/CppCheckResult}'
        }
        stage('Documentation')
        {
            sh 'doxygen Doxyfile'
            publishHTML([allowMissing: false, alwaysLinkToLastBuild: false, keepAll: false, reportDir: 'doc/html/', reportFiles: 'index.html', reportName: 'Doxygen'])
        }
        stage('Parse warnings')
        {
            warnings canComputeNew: false, canResolveRelativePaths: false, categoriesPattern: '', defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '', parserConfigurations: [[parserName: 'Doxygen', pattern: 'doxylog.txt']], unHealthy: ''
            sh returnStatus: true, script: 'rm doxylog.txt'
            warnings canComputeNew: false, canResolveRelativePaths: false, categoriesPattern: '', defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '', parserConfigurations: [[parserName: 'GNU C Compiler 4 (gcc)', pattern: 'build/buildlog.txt']], unHealthy: ''
            sh returnStatus: true, script: 'rm build/buildlog.txt'
        }
    }

}
