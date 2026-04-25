#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <fstream>
#include <iostream>

//NOTE: To RUN the project, make sure you have the following environment variables set, since the AWS SDK uses them:
//AWS_ACCESS_KEY
//AWS_SECRET_ACCESS_KEY
//AWS_DEFAULT_REGION
//For example, you may export them in your terminal prior to running the program:
//  export AWS_DEFAULT_REGION=us-east-2
//etc.
int main() {
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    {
        Aws::S3::S3Client s3_client;

        const std::string bucketName = "path-of-the-phoenix";

        // --- Upload ---
        const std::string uploadObjectName = "uploaded file.txt";
        const std::string uploadFileName   = "local file.txt";

        Aws::S3::Model::PutObjectRequest putRequest;
        putRequest.SetBucket(bucketName);
        putRequest.SetKey(uploadObjectName);

        auto input_data = Aws::MakeShared<Aws::FStream>(
            "UploadTag",
            uploadFileName.c_str(),
            std::ios_base::in | std::ios_base::binary
        );

        if (!input_data->good()) {
            std::cerr << "Error opening file for upload: " << uploadFileName << "\n";
            return 1;
        }

        putRequest.SetBody(input_data);

        auto putOutcome = s3_client.PutObject(putRequest);

        if (!putOutcome.IsSuccess()) {
            std::cerr << "Upload failed: "
                      << putOutcome.GetError().GetMessage() << "\n";
        } else {
            std::cout << "Upload successful!\n";
        }

        // --- Download ---
        const std::string downloadObjectName = "to download.txt"; // must exist in S3
        const std::string downloadFileName   = "downloaded file.txt";

        Aws::S3::Model::GetObjectRequest getRequest;
        getRequest.SetBucket(bucketName);
        getRequest.SetKey(downloadObjectName);

        auto getOutcome = s3_client.GetObject(getRequest);

        if (!getOutcome.IsSuccess()) {
            std::cerr << "Download failed: "
                      << getOutcome.GetError().GetMessage() << "\n";
        } else {
            std::cout << "Download successful!\n";

            // Write S3 object stream to local file
            Aws::OFStream local_file;
            local_file.open(downloadFileName, std::ios::out | std::ios::binary);

            local_file << getOutcome.GetResult().GetBody().rdbuf();
        }
    }
    Aws::ShutdownAPI(options);
    return 0;
}
